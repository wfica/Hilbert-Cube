#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>

#define PI 3.14159265

double s,u,d,fi,psi, TRANSLACJA[3];
int n;


struct matrix
{
	double tab[3][3];
};

struct matrix *MX, *MY, *MZ, *MOX, *MOY, *MROT;

void print(struct matrix * M)
{
	fprintf(stderr, "Macierz:\n");
	for(int i=0; i<3; ++i)
	{
		for(int j=0; j<3; ++j)
			fprintf(stderr, "%lf ", M->tab[i][j]);
		fprintf(stderr, "\n");
	}
}

void set_matrix( struct matrix * Mptr, char axis )
{
	if(axis == 'x' )
	{
		Mptr->tab[0][0]=1;  Mptr->tab[0][1]=0;  Mptr->tab[0][2]=0;
		Mptr->tab[1][0]=0;  Mptr->tab[1][1]=0;  Mptr->tab[1][2]=-1;
		Mptr->tab[2][0]=0;  Mptr->tab[2][1]=1;  Mptr->tab[2][2]=0;
	
	}
	else if( axis == 'y' )
	{
		Mptr->tab[0][0]=0;  Mptr->tab[0][1]=0;  Mptr->tab[0][2]=1;
		Mptr->tab[1][0]=0;  Mptr->tab[1][1]=1;  Mptr->tab[1][2]=0;
		Mptr->tab[2][0]=-1;  Mptr->tab[2][1]=0;  Mptr->tab[2][2]=0;
	}
	else if( axis == 'z' )
	{
		Mptr->tab[0][0]=0;  Mptr->tab[0][1]=-1;  Mptr->tab[0][2]=0;
		Mptr->tab[1][0]=1;  Mptr->tab[1][1]=0;  Mptr->tab[1][2]=0;
		Mptr->tab[2][0]=0;  Mptr->tab[2][1]=0;  Mptr->tab[2][2]=1;
	}
	else if( axis == 'i' )
	{
		Mptr->tab[0][0]=1;  Mptr->tab[0][1]=0;  Mptr->tab[0][2]=0;
        	Mptr->tab[1][0]=0;  Mptr->tab[1][1]=1;  Mptr->tab[1][2]=0;
	        Mptr->tab[2][0]=0;  Mptr->tab[2][1]=0;  Mptr->tab[2][2]=1;
	}
	else if( axis == 'o')
	{
		Mptr->tab[0][0]=0;  Mptr->tab[0][1]=0;  Mptr->tab[0][2]=0;
        	Mptr->tab[1][0]=0;  Mptr->tab[1][1]=0;  Mptr->tab[1][2]=0;
	        Mptr->tab[2][0]=0;  Mptr->tab[2][1]=0;  Mptr->tab[2][2]=0;
	}
	else if( axis == 'A' ) /// MOX
	{
		Mptr->tab[0][0]=1;  Mptr->tab[0][1]=0;  Mptr->tab[0][2]=0;
        	Mptr->tab[1][0]=0;  Mptr->tab[1][1]=cos(fi);  Mptr->tab[1][2]=-sin(fi);
	        Mptr->tab[2][0]=0;  Mptr->tab[2][1]=sin(fi);  Mptr->tab[2][2]=cos(fi);
	}
	else if( axis == 'B' ) /// MOY
	{
		Mptr->tab[0][0]=cos(psi);  Mptr->tab[0][1]=0;  Mptr->tab[0][2]=sin(psi);
        	Mptr->tab[1][0]=0;  Mptr->tab[1][1]=1;  Mptr->tab[1][2]=0;
	        Mptr->tab[2][0]=-sin(psi);  Mptr->tab[2][1]=0;  Mptr->tab[2][2]=cos(psi);
	}
}

void mult_matrices( struct matrix *Aptr, struct matrix *Bptr, struct matrix *Resptr)
{
	for(int i=0; i<3; ++i)
		for(int j=0; j<3; ++j)
		{
			Resptr->tab[i][j]=0;
			for(int k=0; k<3;++k)
				Resptr->tab[i][j] = (Resptr->tab[i][j]) + (Aptr->tab[i][k])*(Bptr->tab[k][j]);
		}
} 

void copy_matrix( struct matrix *S, struct matrix *T)
{
	for(int i=0; i<3;++i)
		for(int j=0; j<3; ++j)
			T->tab[i][j] = S->tab[i][j];
}


struct point
{
	double coord[3];
};

void set_point(struct point * Pptr, double x, double y, double z)
{
	Pptr->coord[0]=x;
	Pptr->coord[1]=y;
	Pptr->coord[2]=z;	
}

void mult_matrix_point( struct matrix *Mptr, struct point *Pptr, struct point *Resptr)
{
	for(int i=0; i<3; ++i)
	{
		Resptr->coord[i]=0;
		for(int j=0; j<3; ++j)
			Resptr->coord[i] += Mptr->tab[i][j]*Pptr->coord[j];
	}
}


struct point *turtle;
struct point *rysik;
double DLUGOSC_ODCINKA;

void go_turtle(struct point *Pptr)
{
//	fprintf(stderr, "line to:");
	
	struct point *wektor, *tmp;
	wektor = (struct point*)malloc(sizeof(struct point));
	tmp = (struct point*) malloc(sizeof(struct point));
	
	for(int i=0; i<3; ++i)
	{
		turtle->coord[i] += Pptr->coord[i];
		wektor->coord[i] = Pptr->coord[i] * DLUGOSC_ODCINKA;
//		fprintf(stderr," %lf",turtle->coord[i]);
	}
	
	
	mult_matrix_point(MROT, wektor, tmp);
//	fprintf(stderr, "\nrysik to: %lf %lf %lf\n", rysik->coord[0], rysik->coord[1], rysik->coord[2]);
	for(int i=0; i<3; ++i)
		rysik->coord[i] += tmp->coord[i];
	

	
	double skala = d/(d + rysik->coord[2]);
	printf("%lf %lf lineto\n", (rysik->coord[0]) * skala + s/2.0, (rysik->coord[1]) * skala + s/2.0);
	
//	fprintf(stderr, "\n");
}

void move(int x, int y, int z, struct matrix *M)
{
	struct point *Pptr;
	Pptr = (struct point *) malloc(sizeof(struct point));
	set_point( Pptr, (double)x, (double)y, (double)z);
	
	struct point *Resptr;
	Resptr = (struct point *) malloc(sizeof(struct point));
	
	mult_matrix_point(M, Pptr, Resptr);
	
	go_turtle(Resptr);
	
	free(Pptr);
	free(Resptr);
}

void rotate( struct matrix *M, int x_No, int y_No, int z_No)
{ /// tylko wokol jednej osi obracamy!!!	
	struct matrix *Res;
	Res = (struct matrix *) malloc(sizeof(struct matrix));
	
	for(int i=0; i<x_No; ++i)
	{
		mult_matrices(MX, M, Res);
		copy_matrix(Res, M);
	}
	for(int i=0; i<y_No; ++i)
	{
		mult_matrices(MY, M, Res);
		copy_matrix(Res, M);
	}
		
	for(int i=0; i<z_No; ++i)
	{
		mult_matrices(MZ, M, Res);
		copy_matrix(Res, M);
	}
	
	copy_matrix(Res, M);
	
	free(Res);
}


void hilbert(int k, struct matrix *M)
{
	if(k==0) return;
	
	struct 	matrix *tmp;
	tmp = (struct matrix *)malloc(sizeof(struct matrix));
	
	struct 	matrix *id;
	id = (struct matrix *)malloc(sizeof(struct matrix));
	
	set_matrix(id, 'i');
	
	//copy_matrix(M, tmp);
	rotate(id, 1,0,0);
	rotate(id, 0,1,0);
	mult_matrices(M, id, tmp);
	hilbert(k-1, tmp);
	
	move( 0, -1, 0, M);
	
	set_matrix(id, 'i');
	//copy_matrix(M, tmp);
	rotate(id, 0,3,0);
	rotate(id, 3,0,0);
	mult_matrices(M, id, tmp);
	hilbert(k-1, tmp);
	
	move(-1, 0, 0, M);
	
	set_matrix(id, 'i');
	//copy_matrix(M, tmp);
	rotate(id, 0,3,0);
	rotate(id, 3,0,0);
	mult_matrices(M, id, tmp);
	hilbert(k-1, tmp);
	
	
	move( 0,1,0, M);
	
	set_matrix(id, 'i');
	//copy_matrix(M, tmp);
	rotate(id, 0,0,1);
	rotate(id, 0,0,1);
	mult_matrices(M, id, tmp);
	hilbert(k-1, tmp);
	
	move(0,0,1, M);
	
	set_matrix(id, 'i');
	//copy_matrix(M, tmp);
	rotate(id, 0,0,3);
	rotate(id, 0,0,3);
	mult_matrices(M, id, tmp);
	hilbert(k-1, tmp);

	move(0,-1,0, M);
	
	set_matrix(id, 'i');
	//copy_matrix(M, tmp);
	rotate(id, 0,1,0);
	rotate(id, 1,0,0);
	mult_matrices(M, id, tmp);
	hilbert(k-1, tmp );
	
	move(1,0,0, M);
	
	set_matrix(id, 'i');
	//copy_matrix(M, tmp);
	rotate(id, 0,1,0);
	rotate(id, 1,0,0);
	mult_matrices(M, id, tmp);
	hilbert(k-1, tmp);
	
	move(0,1,0, M);
	
	set_matrix(id, 'i');
	//copy_matrix(M, tmp);
	rotate(id, 3,0,0);
	rotate(id, 0,3,0);
	mult_matrices(M, id, tmp);
	hilbert(k-1, tmp);
	
	free(tmp);
}



int main(int argc, char *argv[])
{
	if(argc != 10) fprintf(stderr, "blad arg\n");	
	
	sscanf(argv[1], "%d", &n);
	sscanf(argv[2], "%lf", &s);
	sscanf(argv[3], "%lf", &u);
	sscanf(argv[4], "%lf", &d);
	sscanf(argv[5], "%lf", &TRANSLACJA[0]);
	sscanf(argv[6], "%lf", &TRANSLACJA[1]);
	sscanf(argv[7], "%lf", &TRANSLACJA[2]);
	sscanf(argv[8], "%lf", &fi);
	sscanf(argv[9], "%lf", &psi);
	
	fi = fi * PI/180.0;
	psi = psi * PI/180.0; /// zamina na radiany
	
	DLUGOSC_ODCINKA = u/( (1<<n) -1);
	
	
	MOX = (struct matrix *) malloc(sizeof(struct matrix));
	MOY = (struct matrix *) malloc(sizeof(struct matrix));
	MROT= (struct matrix *) malloc(sizeof(struct matrix));
	
	set_matrix(MOX, 'A');
	set_matrix(MOY, 'B');
	mult_matrices(MOX, MOY, MROT);
	
	free(MOX); 
	free(MOY); 
	
//	fprintf(stderr, "dl_odc = %lf\n", DLUGOSC_ODCINKA);
//	print(MROT);
	printf("%%!PS-Adobe-2.0 EPSF-2.0\n%%%%BoundingBox: %d %d %d %d\nnewpath\n%lf %lf moveto\n", 0, 0, (int)s, (int)s, s/2.0 + TRANSLACJA[0] * d/(d + TRANSLACJA[2]), s/2.0 + TRANSLACJA[1] * d/(d + TRANSLACJA[2]));
	
	turtle = (struct point*)malloc(sizeof(struct point));
	set_point(turtle, 0,0,0);
	
	rysik = (struct point*) malloc(sizeof(struct point));
	set_point(rysik, TRANSLACJA[0],TRANSLACJA[1],TRANSLACJA[2]);
	
	struct matrix *M;
	M = (struct matrix *)malloc(sizeof(struct matrix));
	
	MX = (struct matrix *) malloc(sizeof(struct matrix));
	MY = (struct matrix *) malloc(sizeof(struct matrix));
	MZ = (struct matrix *) malloc(sizeof(struct matrix));
	
	set_matrix(M, 'i');
	
	set_matrix(MX, 'x');
	set_matrix(MY, 'y');
	set_matrix(MZ, 'z');
	
	hilbert(n, M);
	
	printf(".02 setlinewidth\nstroke\nshowpage\n%%%%Trailer\n%%EOF");
}
