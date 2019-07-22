#ifndef __EOS_CC
#define __EOS_CC
#include "eos.h"




void EOS::freegascalc_onespecies_finitewidth(CboseMap &npidens,CboseMap &npiP,CboseMap &npiepsilon,CboseMap &npidedt,CparameterMap *parmap,CresInfo *resinfo,double T,double resmass, double m1, double m2, double width, double reswidth_alpha,double spin_deg,
                                              double minmass,double &epsilon,double &P,double &dens,double &sigma2,double &dedt,double &maxweight){

    double kr,k,E0,rho,rho_0,gammas,n0,res_dens,weight,avg_weight,normal;
    double sum=0.0,esum=0.0,psum=0.0,dsum=0.0,sigsum=0.0,dedtsum=0.0;
    double gamma=0;
    int N = 10;
    // E_S0 are E' values where E'=E-resmass
    double E_S0;
    double E;

    resmass=resinfo->mass;
    width=resinfo->width;
    minmass=resinfo->minmass;
    m1=resinfo->branchlist[0]->resinfo[0]->mass;
    m2=resinfo->branchlist[0]->resinfo[1]->mass;

    //if(resinfo->code==213)
    //printf("line 25 ---- PID=%d\n",resinfo->code);

    // E0 = minmass if minmass declared already in resonance.cc
    E0=m1+m2;
    maxweight=0.0;
    res_dens=gsl_sf_bessel_Kn(2,resmass/T)*resmass*resmass*T/(2*PI*PI*pow(HBARC,3.0));
    kr=sqrt(abs(pow((resmass*resmass-m1*m1-m2*m2),2.0)-4.0*m1*m1*m2*m2))/(2.0*resmass);

    //if(resinfo->code==30223)
    //{ printf("---- PID=%d,resmass=%g,minmass=%g,m1=%g,m2=%g ---\n",resinfo->code,resmass,minmass,m1,m2); }

    for(int n=0;n<N;n++)
    {
        double Sum_E=(n+0.5)/N;
        E_S0 = 0.5*width*tan(PI*(Sum_E - .5));
        E = E_S0+resmass;

        //if(resinfo->code==223)
        //{ printf("n=%d,E=%g\n",n,E); }

        if((E_S0+resmass)>=minmass)
        {
            //if(resinfo->code==229)
            //{ printf("n=%d,E=%g\n",n,E); }

            if(resinfo->branchlist[0]->resinfo[0]->decay==true || resinfo->branchlist[0]->resinfo[1]->decay==true)
            {
                double ma,mb,ma1,ma2,ma_pole,ma_0,ma_min,sum_ma,na,ma_gamma,ma_width;
                double mb1,mb2,mb_pole,mb_0,mb_min,sum_mb,nb,mb_gamma,mb_width,Emb,Ema;

                double form_lambda,ma_kr,ma_k,ma_rho,ma_rho0,suma,rho_width,rho_width_0,spectsumb,spectsumb0,kr_ab,k_ab,s0;
                double mb_kr,mb_k,mb_rho,mb_rho0,sumb,spectsum,spectsum0;

                if(resinfo->branchlist[0]->resinfo[0]->decay==true) // 1st daughter in 1 daughter decay and 2 daughter decay
                {   ma_min=resinfo->branchlist[0]->resinfo[0]->minmass;
                    ma_pole=resinfo->branchlist[0]->resinfo[0]->mass;
                    mb_pole=resinfo->branchlist[0]->resinfo[1]->mass;
                    //if(resinfo->branchlist[0]->resinfo[0]->code==229)
                    //{mb_width=.001;}
                    ma_width=resinfo->branchlist[0]->resinfo[0]->width;
                    ma1=resinfo->branchlist[0]->resinfo[0]->branchlist[0]->resinfo[0]->mass;
                    ma2=resinfo->branchlist[0]->resinfo[0]->branchlist[0]->resinfo[1]->mass;
                    if(resinfo->branchlist[0]->resinfo[1]->decay==true){
                        mb_min=resinfo->branchlist[0]->resinfo[1]->minmass;
                        //if(resinfo->branchlist[0]->resinfo[1]->code==223)
                        //{mb_width=.001;}
                        mb_width=resinfo->branchlist[0]->resinfo[1]->width;
                        mb1=resinfo->branchlist[0]->resinfo[1]->branchlist[0]->resinfo[0]->mass;
                        mb2=resinfo->branchlist[0]->resinfo[1]->branchlist[0]->resinfo[1]->mass;
                        mb_kr=sqrt(abs(pow((mb_pole*mb_pole-mb1*mb1-mb2*mb2),2.0)-4.0*mb1*mb1*mb2*mb2))/(2.0*mb_pole);
                        Emb = E - mb_min;
                        //if(resinfo->code==229)
                        //{ printf("mb_min=%g,E=%g\n",mb_min,E); }
                    }
                    else{
                        mb=resinfo->branchlist[0]->resinfo[1]->mass;
                        Emb = E - mb;
                        //if(resinfo->code==223)
                        //{ printf("mb=%g,E=%g\n",mb,E); }
                    }
                    if(m1==776 && m2==138) { form_lambda=0.8; }
                    else if(resinfo->branchlist[0]->resinfo[1]->decay) { form_lambda=0.6; }
                    else if(resinfo->branchlist[0]->resinfo[0]->baryon==0) { form_lambda=1.6; }
                    else {form_lambda=2.0;}
                }
                else // 2nd daughter in 1 daughter decay
                {   ma_min=resinfo->branchlist[0]->resinfo[1]->minmass;
                    ma_pole=resinfo->branchlist[0]->resinfo[1]->mass;
                    mb=resinfo->branchlist[0]->resinfo[0]->mass;
                    ma_width=resinfo->branchlist[0]->resinfo[1]->width;
                    ma1=resinfo->branchlist[0]->resinfo[1]->branchlist[0]->resinfo[0]->mass;
                    ma2=resinfo->branchlist[0]->resinfo[1]->branchlist[0]->resinfo[1]->mass;
                    Emb = E - mb;
                    //if(resinfo->code==223)
                    //{ printf("mb=%g,E=%g\n",mb,E); }
                    if(m1==776 && m2==138) { form_lambda=0.8; }
                    else if(resinfo->branchlist[0]->resinfo[0]->decay) { form_lambda=0.6; }
                    else if(resinfo->branchlist[0]->resinfo[1]->baryon==0) { form_lambda=1.6; }
                    else {form_lambda=2.0;}
                }

                //if(resinfo->code==223)
                //{ printf("---- ma_min=%g,(E-mb)=%g,ma_pole=%g,ma1=%g,ma2=%g,mb=%g ---\n",ma_min,Emb,ma_pole,ma1,ma2,mb); }

                if(ma_min>=Emb) continue;

                ma_kr=sqrt(abs(pow((ma_pole*ma_pole-ma1*ma1-ma2*ma2),2.0)-4.0*ma1*ma1*ma2*ma2))/(2.0*ma_pole);
                suma=0.0;
                int Na=100;
                int ma_counter;
                ma_counter = 0;

                for(int na=0;na<Na;na++)
                {
                    double sum_ma=(na+0.5)/Na;
                    ma_0 = 0.5*ma_width*tan(PI*(sum_ma - .5));
                    ma = ma_0+ma_pole;
                    Ema = E - ma;

                    //if(resinfo->code==229)
                    //{ printf("na=%d,ma=%g\n",na,ma); }

                    if(ma>=ma_min && ma<=Emb)
                    {

                        ma_k=sqrt(abs(pow((ma*ma-ma1*ma1-ma2*ma2),2.0)-(4.0*ma1*ma1*ma2*ma2)))/(2.0*ma);
                        ma_gamma=ma_width*(ma_pole/ma)*((ma_k*ma_k*ma_k)/(ma_kr*ma_kr*ma_kr))*((ma_kr*ma_kr+HBARC*HBARC)/(ma_k*ma_k+HBARC*HBARC));
                        ma_rho=(2.0)/(ma_width*PI)*0.25*ma_gamma*ma_gamma/((0.25*ma_gamma*ma_gamma)+(ma_pole-ma)*(ma_pole-ma));
                        ma_rho0 = (1/PI)*(ma_width/2.0)/(0.25*ma_width*ma_width+ma_0*ma_0);


                        if(resinfo->branchlist[0]->resinfo[0]->decay==true && resinfo->branchlist[0]->resinfo[1]->decay==true)
                        {

                            if(mb_min>=Ema) continue;

                            mb_kr=sqrt(abs(pow((mb_pole*mb_pole-mb1*mb1-mb2*mb2),2.0)-4.0*mb1*mb1*mb2*mb2))/(2.0*mb_pole);
                            sumb=0.0;
                            int Nb=100;
                            int mb_counter;
                            mb_counter = 0;

                            //if(resinfo->code==30223)
                            //{ printf("---- PID=%d,daughter PID=%d ---\n",resinfo->code,resinfo->branchlist[0]->resinfo[1]->code); }

                            for(int nb=0;nb<Nb;nb++)
                            {
                                double sum_mb=(nb+0.5)/Nb;
                                mb_0 = 0.5*mb_width*tan(PI*(sum_mb - .5));
                                mb = mb_0+mb_pole;

                                //if(resinfo->code==229)
                                //{ printf("nb=%d,mb=%g\n",nb,mb); }

                                //if(resinfo->code==30223)
                                //printf("mb_min=%g,\n",mb_min);

                                if(mb>=mb_min && mb<=Ema)
                                {
                                    mb_k=sqrt(abs(pow((mb*mb-mb1*mb1-mb2*mb2),2.0)-(4.0*mb1*mb1*mb2*mb2)))/(2.0*mb);
                                    mb_gamma=mb_width*(mb_pole/mb)*((mb_k*mb_k*mb_k)/(mb_kr*mb_kr*mb_kr))*((mb_kr*mb_kr+HBARC*HBARC)/(mb_k*mb_k+HBARC*HBARC));
                                    mb_rho=(2.0)/(mb_width*PI)*0.25*mb_gamma*mb_gamma/((0.25*mb_gamma*mb_gamma)+(mb_pole-mb)*(mb_pole-mb));
                                    mb_rho0 = (1/PI)*(mb_width/2.0)/(0.25*mb_width*mb_width+mb_0*mb_0);

                                    //if(resinfo->code==229)
                                    //printf("mb_min=%g,mb1=%g,mb2=%g\n",mb_min,mb1,mb2);

                                    kr_ab=sqrt(abs(pow((resmass*resmass-ma*ma-mb*mb),2.0)-(4.0*ma*ma*mb*mb)))/(2.0*resmass);
                                    k_ab=sqrt(abs(pow((E*E-ma*ma-mb*mb),2.0)-(4.0*ma*ma*mb*mb)))/(2.0*E);
                                    s0=ma+mb;
                                    rho_width=(k_ab*k_ab*k_ab)/(E*(k_ab*k_ab+HBARC*HBARC))*((pow(form_lambda,4.0)+0.25*pow((s0-resmass*resmass),2.0))/(pow(form_lambda,4.0)+pow((E*E-0.5*(s0+resmass*resmass)),2.0)));
                                    rho_width_0=(kr_ab*kr_ab*kr_ab)/(resmass*(kr_ab*kr_ab+HBARC*HBARC));

                                    sumb+=mb_rho/mb_rho0;
                                    spectsumb+=rho_width*mb_rho;
                                    spectsumb0+=rho_width_0*mb_rho;
                                    mb_counter++;

                                    //if(resinfo->code==30223)
                                    //printf("{%g, %g},\n",mb,mb_rho);

                                }

                            }
                            if(mb_counter == 0) continue;
                            double avg_weight_mb=sumb/Nb;
                            double normal_mb=1.0/avg_weight_mb;
                            double spectb=normal_mb*spectsumb/Nb;
                            double spectb0=normal_mb*spectsumb0/Nb;

                            suma+=ma_rho/ma_rho0;
                            spectsum+=spectb*ma_rho;
                            spectsum0+=spectb0*ma_rho;
                            ma_counter++;

                            //if(resinfo->code==229)
                            //{ printf("ma_k=%g,ma_gamma=%g,ma_rho=%g,ma_rho0=%g\n",ma_k,ma_gamma,ma_rho,ma_rho0); }
                        }
                        else{
                            ma_k=sqrt(abs(pow((ma*ma-ma1*ma1-ma2*ma2),2.0)-(4.0*ma1*ma1*ma2*ma2)))/(2.0*ma);
                            ma_gamma=ma_width*(ma_pole/ma)*((ma_k*ma_k*ma_k)/(ma_kr*ma_kr*ma_kr))*((ma_kr*ma_kr+HBARC*HBARC)/(ma_k*ma_k+HBARC*HBARC));
                            ma_rho=(2.0)/(ma_width*PI)*0.25*ma_gamma*ma_gamma/((0.25*ma_gamma*ma_gamma)+(ma_pole-ma)*(ma_pole-ma));
                            ma_rho0 = (1/PI)*(ma_width/2.0)/(0.25*ma_width*ma_width+ma_0*ma_0);

                            kr_ab=sqrt(abs(pow((resmass*resmass-ma*ma-mb*mb),2.0)-(4.0*ma*ma*mb*mb)))/(2.0*resmass);
                            k_ab=sqrt(abs(pow((E*E-ma*ma-mb*mb),2.0)-(4.0*ma*ma*mb*mb)))/(2.0*E);
                            s0=ma+mb;
                            rho_width=(k_ab*k_ab*k_ab)/(E*(k_ab*k_ab+HBARC*HBARC))*((pow(form_lambda,4.0)+0.25*pow((s0-resmass*resmass),2.0))/(pow(form_lambda,4.0)+pow((E*E-0.5*(s0+resmass*resmass)),2.0)));
                            rho_width_0=(kr_ab*kr_ab*kr_ab)/(resmass*(kr_ab*kr_ab+HBARC*HBARC));

                            suma+=ma_rho/ma_rho0;
                            spectsum+=rho_width*ma_rho/ma_rho0;
                            spectsum0+=rho_width_0*ma_rho/ma_rho0;
                            ma_counter++;

                            //if(resinfo->code==223)
                            //{ printf("ma_k=%g,ma_gamma=%g,ma_rho=%g,ma_rho0=%g\n",ma_k,ma_gamma,ma_rho,ma_rho0); }
                        }
                    }
                }

                if(ma_counter == 0) continue;
                double avg_weight_ma=suma/Na;
                double normal_ma=1.0/avg_weight_ma;
                double spect=normal_ma*spectsum/Na;
                double spect0=normal_ma*spectsum0/Na;
                gamma=width*spect/spect0;
                //if(resinfo->code==229)
                //printf("gamma=%g\n",gamma);
            }


            else{
                k=sqrt(abs(pow((E*E-m1*m1-m2*m2),2.0)-(4.0*m1*m1*m2*m2)))/(2.0*E);
                if(spin_deg<1.001)
                {gamma=width*(resmass/E)*(k/kr); }
                else {gamma=width*(resmass/E)*((k*k*k)/(kr*kr*kr))*((kr*kr+HBARC*HBARC)/(k*k+HBARC*HBARC));}
                //printf("rho=%g,PID=%d\n",rho,resinfo->code);
            }

            rho=(2.0)/(width*PI)*0.25*gamma*gamma/((0.25*gamma*gamma)+(resmass-E)*(resmass-E));
            rho_0 = (1/PI)*(width/2.0)/(0.25*width*width+E_S0*E_S0);
            double corr = rho/rho_0;
            /*
             if(resinfo->code==30223)
             {
             //printf("{%g, %g},\n",E,corr);

             printf("{%g, %g},\n",E,rho);
             }
             */ freegascalc_onespecies(npidens,npiP,npiepsilon,npidedt,parmap,resinfo,T,E,epsilon,P,dens,sigma2,dedt);
            weight=rho*dens/(rho_0*res_dens);
            if(weight>maxweight)
                maxweight=weight;
            esum+=epsilon*rho/rho_0;
            psum+=P*rho/rho_0;
            dsum+=dens*rho/rho_0;
            sigsum+=sigma2*rho/rho_0;
            dedtsum+=dedt*rho/rho_0;
            sum+=rho/rho_0;
        }
    }

        avg_weight=sum/N;
        normal=1.0/avg_weight;
        //if(resinfo->code==22214)
        //{printf("%g\n",normal);}
        epsilon=normal*esum/N;
        P=normal*psum/N;
        dens=normal*dsum/N;
        //printf("line 142 --- %g\n",dens);
        sigma2=normal*sigsum/N;
        dedt=normal*dedtsum/N;
}

void EOS::freegascalc_onespecies(CboseMap &npidens,CboseMap &npiP,CboseMap &npiepsilon,CboseMap &npidedt,CparameterMap *parmap,CresInfo *resinfo,double T,double m,double &epsilon,double &P,double &dens,double &sigma2,double &dedt){
    const double prefactor=1.0/(2.0*PI*PI*pow(HBARC,3));
    double k0,k1,z,k0prime,k1prime,m2,m3,m4,t2,t3,I1,I2,Iomega;
    bool pion;
    int n;
    m2=m*m;
    m3=m2*m;
    m4=m2*m2;
    //this whole if/else loop does all the bose correction checks
    if (resinfo->code==-211 || resinfo->code==211 || resinfo->code==111) {
        pion=true;
        if (parmap->getB("BOSE_CORR",false)) {
            n=parmap->getI("N_BOSE_CORR",1);
        }
        else n=1;
    }
	else {
        n=1;
        pion=false;
    }

    z=m/T;
    if(z>1000.0){
        P=epsilon=dens=dedt=0.0;
        printf("z is huge=%g, m=%g, t=%g\n",z,m,T);
    }
    else{
        if(z<0.0){
            printf("___z=%g,m=%g,T=%g ___\n",z,m,T);
            exit(1);
        }
        P=epsilon=dens=dedt=0.0;
        for (int i=1;i<=n;i++) {
            double Ti=T/i;
            t2=Ti*Ti;
            t3=t2*Ti;
            z=m/Ti;
            k0=Bessel::K0(z);
            k1=Bessel::K1(z);
            //k0=boost::math::cyl_bessel_k(0.0,z);
            //k1=boost::math::cyl_bessel_k(1.0,z);

            double temp=prefactor*(m2*t2*k0+2.0*m*t3*k1);
            P+=temp;
            if(pion) npiP[resinfo->code].push_back(temp);

            dens+=temp/Ti;
            if(pion) npidens[resinfo->code].push_back(temp/Ti);

            temp=prefactor*(3.0*m2*t2*k0+(m3*Ti+6.0*m*t3)*k1);
            epsilon+=temp;
            if(pion) {
                //printf("npiepsilon[code][%d]=%lf\n",i-1,npiepsilon[resinfo->code][i-1]);
                npiepsilon[resinfo->code].push_back(temp);
            }

            k0prime=-k1;
            k1prime=-k0-k1/z;

            temp=prefactor*(6.0*m2*Ti*k0+(m3+18.0*m*t2)*k1-3.0*m3*k0prime-((m4/Ti)+6.0*m2*Ti)*k1prime);
            dedt+=temp;
            if(pion) npidedt[resinfo->code].push_back(temp);
        }
        z=m/T;
        Iomega=exp(-z)/(30.0*PI*PI*HBARC*HBARC*HBARC);
        I1=pow(m,1.5)*pow(T,3.5)*7.5*sqrt(2.0*PI);
        I2=24.0*pow(T,5);
        sigma2=Iomega*(I1+I2+0.5*sqrt(I1*I2));  // this is an approximation (+/-2%) to messy integral
        //printf("___z=%g,m=%g,T=%g ___\n",z,m,T);
    }
}


#endif
