#ifndef __PART_CC__
#define __PART_CC__

#include "pratt_sampler/part.h"

Cpart::Cpart(){
	msquared=0.0;
	pid=0;
	for(int alpha=0;alpha<4;alpha++){
		r[alpha]=p[alpha]=0.0;
	}
}

Cpart::~Cpart(){
}

void Cpart::Print(){
	printf("________________ PART INFO FOR PART, pid=%d _____________________________\n",pid);
	printf("m^2=%g\\p=(%g,%g,%g,%g)\n",msquared,p[0],p[1],p[2],p[3]);
	printf("r=(%g,%g,%g,%g)\n",r[0],r[1],r[2],r[3]);
}

double Cpart::GetMass(){
	if(pid==22)
		return 0.0;
	else
		return sqrt(msquared);
}

void Cpart::SetMsquared(){
	msquared=p[0]*p[0]-p[1]*p[1]-p[2]*p[2]-p[3]*p[3];
}

void Cpart::Setp0(){
	p[0]=sqrt(p[1]*p[1]+p[2]*p[2]+p[3]*p[3]+msquared);
}

void Cpart::Boost(FourVector &u){
	BoostP(u);
	BoostR(u);
}

void Cpart::BoostP(FourVector &u){
	int alpha;
	FourVector pprime;
	Misc::Boost(u,p,pprime);
	for(alpha=0;alpha<4;alpha++)
		p[alpha]=pprime[alpha];
}

void Cpart::BoostR(FourVector &u){
	int alpha;
	FourVector rprime;
	Misc::Boost(u,r,rprime);
	for(alpha=0;alpha<4;alpha++)
		r[alpha]=rprime[alpha];
}

CpartList::CpartList(CparameterMap *parmap){
	nparts_blocksize=parmap->getI("SAMPLER_NPARTS_BLOCKSIZE",2000);
	partvec.resize(nparts_blocksize);
	nparts=0;
}

CpartList::~CpartList(){
	partvec.clear();
	partvec.shrink_to_fit();
}

Cpart* CpartList::GetPart(){
	if(partvec.size()==nparts){
		printf("resizing partvec, old size=%lu\n",partvec.size());
		partvec.resize(partvec.size()+nparts);
	}
	nparts+=1;
	return &partvec[nparts];
}
void CpartList::Kill(){
	partvec.clear();
	partvec.shrink_to_fit();
	nparts=0;
}

void CpartList::Reset(){
	nparts=0;
}

void CpartList::WriteParts(string filename){
	FILE *fptr=fopen(filename.c_str(),"w");
	for(int ipart=0;ipart<nparts;ipart++){
		fprintf(fptr,"%5d %15.10e %15.10e %15.10e %15.10e %15.10e %15.10e %15.10e %15.10e %15.10e\n",
		partvec[ipart].pid,partvec[ipart].msquared,partvec[ipart].p[0],partvec[ipart].p[1],partvec[ipart].p[2],partvec[ipart].p[3],
		partvec[ipart].r[0],partvec[ipart].r[1],partvec[ipart].r[2],partvec[ipart].r[3]);
	}
	fclose(fptr);
}

int CpartList::CountResonances(int pid){
	int count=0,ipart;
	for(ipart=0;ipart<nparts;ipart++){
		if(partvec[ipart].pid==pid)
			count+=1;
	}
	return count;
}

double CpartList::SumEnergy(){
	double energy=0.0,ipart;
	for(ipart=0;ipart<nparts;ipart++){
		energy+=partvec[ipart].p[0];
	}
	return energy;
}

double CpartList::SumEnergy(int pid){
	double energy=0.0,ipart;
	for(ipart=0;ipart<nparts;ipart++){
		if(partvec[ipart].pid==pid)
			energy+=partvec[ipart].p[0];
	}
	return energy;
}

void CpartList::AddPart(int pidset,FourVector &pset,FourVector &rset){
	if(partvec.size()==nparts){
		//printf("resizing partvec, old size=%lu\n",partvec.size());
		partvec.resize(partvec.size()+nparts);
	}
	partvec[nparts].pid=pidset;
	for(int alpha=0;alpha<4;alpha++){
		partvec[nparts].p[alpha]=pset[alpha];
		partvec[nparts].r[alpha]=rset[alpha];
	}
	partvec[nparts].SetMsquared();
	nparts+=1;
}

#endif
