#include"heading.h"
#include"log_normal.c"
#include"popStruc.c"
#include"parameters.c"
#include"interaction.c"
#include"mosqStruc.c"
#include"ProbDist.c"
#include"functions.c"
#include"pregnant.c"
#include"vaccination.c"
//# include <omp.h>

main(int argc,char *argv[]){

      //struct Human *H;
      //struct Mosquito *M;
    int i,j,controlSeason,timecount,d,rn,susc,latent,symp,asymp,sympiso,rec,numberofseason,sim,begin,end,seed,UpDating[4],core,NumberInAge[7],**PregAgeGroup,number_of_microcephaly,aux2,aux3,aux4,NumPregInf[4],VacVector[4];
    float dist[2][60],rd,distAge[60],soma,distPreg[7];
    FILE *ar,*arq,*arqu;
    char name[30],namecore[30];

    arqu=fopen("distPreg.dat","r");
    ar=fopen("summer.txt","r");
    arq=fopen("winter.txt","r");

    for(i=0;i<60;i++) fscanf(ar,"%f",&dist[0][i]);//life span dist for summer
    for(i=0;i<30;i++) fscanf(arq,"%f",&dist[1][i]);//life span dist for winter
    for(i=0;i<7;i++) fscanf(arqu,"%f",&distPreg[i]);//pregnant dist
    fclose(arq);
    fclose(ar);
    fclose(arqu);


    begin=atoi(argv[1]);
    end=atoi(argv[2]);
    seed=atoi(argv[3]);
    core=atoi(argv[4]);

    printf("%d %d %d %d\n",begin,end,seed,core);
    srand(seed);

    PregAgeGroup=Alocar_matriz_inteira(7,N);
  /////starting simulations

  //getchar();
  sprintf(namecore,"R19/teste6/NoM%d.dat",core);

  arq=fopen(namecore,"w");

  for(sim=begin;sim<=end;sim++){
    struct Human *H=malloc(N*sizeof(struct Human));//Hvector(0,N-1);
    struct Mosquito *M=malloc(NM*sizeof(struct Mosquito));//Mvector(0,NM-1);

    sprintf(name,"R19/teste6/tt%d.dat",sim);//name of the file

    for(i=0;i<4;i++){
      NumPregInf[i]=0;
      VacVector[i]=0;
    }

     number_of_microcephaly=0;
    printf("simulacao=%d\n",sim);

    for(i=0;i<60;i++) distAge[i]=0;



    for(i=0;i<NM;i++){
      set_up_mosquito(M,i,dist);
      distAge[M[i].ageofdeath-1]+=1.0;//create initial age
    }


    ///CREATING THE INITIAL AGE OF THE MOSQUITOS
    distAge[0]/=NM;
    for(i=1;i<60;i++){
      distAge[i]/=NM;
      distAge[i]+=distAge[i-1];
    }
    //getchar();
    for(i=0;i<NM;i++){
      rd=(float)rand()/(RAND_MAX+1.);
      for(j=0;j<60;j++) if(rd<distAge[j]) break;
      M[i].age=j+1;
    }
    /////////////////////////////////////////////////////////////////
    // getchar();
    /*-------------------------------*/


    set_up_humans(H);
    sexual_tree(H);
    setup_pregnant(H,NumberInAge,PregAgeGroup,distPreg);
    vaccination(H,VacVector);

    for(i=0;i<N;i++){
        if(H[i].Vaccination) H[i].health=6;
    }
   // printf("ok\n");
    ///Setting up the random latent person;
    for(i=0;i<initial_latent;i++){

      rn=N*((float)rand()/(RAND_MAX+1.));
      //printf("initial latent=%d %d\n",N,rn);
      if(H[rn].health==1){
          H[rn].health=2;

          d=log_normal_sample(exp(h_lognormal_latent_scale),exp(h_lognormal_latent_shape),rand());
          //printf("d=%d\n",d);
          H[rn].statetime=max(4,min(d,h_latency_max));
          H[rn].timeinstate=0;
          H[rn].swap=-1;
          ProbMicrocephaly(H,rn,NumPregInf);
      }
    }

   // i=5690;
    //printf("%d %d %d %d %d %d %d\n",H[i].health,H[i].Vaccination,H[i].pregnant,H[i].timepregnant,H[i].timevaccination,H[i].age,H[i].PregnantAgeGroup);
    //getchar();
    //printf("%d %d %d",NumPregInf[0],NumPregInf[1],NumPregInf[3]);
    //getchar();
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    ar=fopen(name,"w");
    season=0;
    controlSeason=1;

    for(i=0;i<4;i++) UpDating[i]=0;

    /////////Here the loop for time must begin
    for(numberofseason=0;numberofseason<4;numberofseason++){
//printf("chegou\n");
//getchar();
      for(timecount=0;timecount<182;timecount++){

            fprintf(ar,"%d %d %d %d %d\n",182*numberofseason+timecount,UpDating[0],UpDating[1],UpDating[2],UpDating[3]);
            for(i=0;i<4;i++) UpDating[i]=0;

            //printf("ok5\n");

                BiteTransmission(H,M);

                Sexual_interaction(H);

                number_of_microcephaly=number_of_microcephaly+update_pregnant(H,NumberInAge,PregAgeGroup,NumPregInf,VacVector);

                increase_timestateH(H,VacVector);

                update_human(H,UpDating,NumPregInf,VacVector);

                increase_timestateM(M);

                update_mosquito(M);

                increase_mosquito_age(M,dist);

               // printf(" %d %d %d %d",182*numberofseason+timecount,NumPregInf[0],NumPregInf[1],NumPregInf[2]);
   // getchar();

        }//close time

        season=season+controlSeason;
          //printf("season=%d\n",season);
        controlSeason*=-1;

    }//close season

    fclose(ar);
    number_of_microcephaly=number_of_microcephaly+CountingLast(H);
    fprintf(arq,"%d %d %d %d %d %d %d %d %d\n",number_of_microcephaly,NumPregInf[0],NumPregInf[1],NumPregInf[2],NumPregInf[3],VacVector[0],VacVector[1],VacVector[2],VacVector[3]);
    //Number of Babies with microcephaly, number of infected pregnants in 1st 2nd and 3rd trimester, number of vaccinated people for age, number of vaccinated pregnant, number of immune vac people, number of immune vac preg

//    printf("END OF SIM\n");
//    aux2=aux3=aux4=0;
//    for(i=0;i<N;i++){
//printf("%d %d %d %d\n",H[i].pregnant,H[i].timepregnant,H[i].age,H[i].gender);
//if(H[i].pregnant==1){
//if(H[i].timepregnant<=90) aux2++;
//else{
//if(H[i].timepregnant<=180) aux3++;
//else aux4++;
//}
//}
////getchar();
//}


   //printf("Numm==%d %d %d %d %d %d\n",aux2,aux3,aux4,aux2+aux3+aux4,NumPregInf[0],NumPregInf[1]);
//
//getchar();

    free(M);
    free(H);
  }//close sim


}


int **Alocar_matriz_inteira(int m,int n) {
  int **v;
  /* ponteiro para a matriz */
  int i;

  /* aloca as linhas da matriz */
  v = (int**) calloc (m,sizeof(int*));
  if (v == NULL) {
    printf ("** Erro: Memoria Insuficiente **");
    return (NULL);
  }
/* aloca as colunas da matriz */
  for ( i = 0; i < m; i++ ) {
    v[i] = (int*) calloc (n, sizeof(int));
    if (v[i] == NULL) {
      printf ("** Erro: Memoria Insuficiente **");
      return
	(NULL);
    }
  }
  return
    (v);
  /* retorna o ponteiro para a matriz */
}

void nrerror(char error_text[])
/* Numerical Recipes standard error handler */

{
  fprintf(stderr,"Numerical Recipes run-time error...\n");
  fprintf(stderr,"%s\n",error_text);
  fprintf(stderr,"...now exiting to system...\n");
  exit(1);
}
