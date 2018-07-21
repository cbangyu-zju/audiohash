#include <cmath>
#include <cstdlib>
#include "Spectral.h"

CSpectral::CSpectral(int winlen)
{
	WindowLen = winlen;
	InitArray();

	p = 5;
	for(int i = 0;i<PSD_LEN;i++)
	{
		psdData[i] = 0;
		dataFreq[i] = 0;
	}
}


CSpectral::~CSpectral(void)
{


}



void CSpectral::InitArray()
{
	psdData = new double [PSD_LEN];
	dataFreq = new double [PSD_LEN];
	dataFFT = new double [WindowLen*2];
}

/* ����Ϊ�ִ���������غ���*/
//burg
void CSpectral::Pburg(double wave[],int fs)
{
	double a[ORDER+1];
	double burg_v;
	double b[1]={1};
	int q = 0;
	Burg(wave,WindowLen,p,a,&burg_v);
	Psd(b,a,q,p,burg_v,(double)fs, psdData, dataFreq, (int)PSD_LEN,1);
}
void CSpectral::Burg(double x[],int n,int p,double a[],double *v)
{
	int i,k;
	double r0,sumd,sumn,*b,*ef,*eb;
	b=(double *)malloc((p+1)*sizeof(double));
	ef=(double *)malloc(n*sizeof(double));
	eb=(double *)malloc(n*sizeof(double));
	a[0]=1.0;
	r0=0.0;
	for(i=0;i<n;i++)
	{r0+=x[i]*x[i]/n;}
	v[0]=r0;
	for(i=1;i<n;i++)
	{
		ef[i]=x[i];
		eb[i-1]=x[i-1];
	}
	for(k=1;k<=p;k++)
	{
		sumn=0.0;
		sumd=0.0;
		for(i=k;i<n;i++)
		{
			sumn+=ef[i]*eb[i-1];
			sumd+=ef[i]*ef[i]+eb[i-1]*eb[i-1];
		}
		a[k]=-2*sumn/sumd;
		for(i=1;i<k;i++)
		{b[i]=a[i]+a[k]*a[k-i];}
		for(i=1;i<k;i++)
		{a[i]=b[i];}
		v[0]=(1.0-a[k]*a[k])*v[0];
		for(i=(n-1);i>=(k+1);i--)
		{
			ef[i]=ef[i]+a[k]*eb[i-1];
			eb[i-1]=eb[i-2]+a[k]*ef[i-1];
		}
	}
	free(b);
	free(ef);
	free(eb);
}

//yulewalk
void CSpectral::Pyulear(double wave[],int fs)
{
	double a[ORDER+1];
	double burg_v;
	double b[1]={1};
	int q = 0;
	double *data;
	data = new double[WindowLen];
	for(int i = 0;i<WindowLen;i++)
	{
		data[i] = wave[i];
	}
	yulewalk(data,WindowLen,p,a,&burg_v);
	Psd(b,a,q,p,-1*burg_v,(double)fs,psdData,dataFreq,(int)PSD_LEN,0);
	for(int i = 0;i<PSD_LEN;i++)
	{
		if(psdData[i]<0)
		{
			psdData[i] = -psdData[i];
		}
	}
}

void CSpectral::yulewalk(double x[],int n,int p,double a[],double *v)
{
	int i,k;
	double s,*r;
	r = (double *)malloc((p+1)*sizeof(double));
	for(k = 0;k<p;k++)
	{
		s = 0.0;
		for(i = 0;i<(n-k);i++)
		{
			s+=x[i]*x[i+k];
		}
		r[k] = s/n;
	}
	levinson(a,r,p,v);
	free(r);
}
void CSpectral::levinson(double a[],double r[],int p,double *v)
{
	int i,k;
	double q,*b;
	b = (double *)malloc((p+1)*sizeof(double));
	a[0] = 1.0;
	a[1] = -r[1]/r[0];
	v[0] = (1-a[1]*a[1])*r[0];
	for(k = 2;k<=p;k++)
	{
		q =0.0;
		for(i = 0;i<k;i++)
		{
			q+=a[i]*r[k-i];
		}
		a[k] = -q/v[0];
		for(i = 1;i<k;i++)
		{
			b[i] = a[i]+a[k]*a[k-i];
		}
		for(i = 1;i<k;i++)
		{
			a[i] = b[i];
		}
		v[0] = (1-a[k]*a[k])*v[0];
	}
	free(b);
}

//Э�����׹���
void CSpectral::Pcovar(double wave[],int fs,int mode)
{
	double a[ORDER+1];
	double burg_v;
	double b[1]={1};
	int q = 0;
	covar(wave,WindowLen,p,a,&burg_v,mode);
	Psd(b,a,q,p,burg_v,(double)fs,psdData,dataFreq,(int)PSD_LEN,0);
}
void CSpectral::covar(double x[],int n, int p,double a[],double *v, int mode)
{
	int i,j,k,m;
	double cc,sum,*c;
	c = (double *)malloc((p*(p+1)/2)*sizeof(double));
	m = 0;
	for(k = 1;k<=p;k++)
		for(j = 1;j<=k;j++)
		{
			c[m] = 0.0;
			for(i = p;i<n;i++)
			{
				c[m] += x[i-j]*x[i-k];
			}
			if(mode == 1)
			{
				for(i = 0;i<(n-p);i++)
				{
					c[m] += x[i+j]*x[i+k];
				}
			}
			m = m+1;
		}
	for(j = 1;j<=p;j++)
		{
			a[j-1] = 0.0;
			for(i=p;i<n;i++)
			{
				a[j-1] -=x[i-j]*x[i];
			}
			if(mode == 1)
			{
				for(i=0;i<(n-p);i++)
				{
					a[j-1] -= x[i+j]*x[i];
				}
			}
		}
	cholesky(c,a,p);
	for(k = (p-1);k>=0;k--)
	{
		a[k+1] = a[k];
	}
	a[0] = 1.0;
	sum = 0.0;
	for(k = 0;k<=p;k++)
		{
			cc = 0.0;
			for(i = p;i<n;i++)
			{
				cc+=x[i]*x[i-k];
			}
			if(mode == 1)
			{
				for(i = 0;i<(n-p);i++)
				{
					cc+=x[i]*x[i+k];
				}
			}
			if(k ==0)
			{
				sum +=cc;
			}
			else
			{
				sum +=cc*a[k]; 
			}
		}
	if(mode == 1)
	{
		v[0] = sum/(2*(n-p));
	}
	else
	{
		v[0] = sum/(n-p);
	}
	free(c);
}
void CSpectral::cholesky(double a[],double b[],int n)
{
	int i,j,k,m;
	double *d,*y,*xl,eps;
	d = (double *)malloc(n*sizeof(double));
	y = (double *)malloc(n*sizeof(double));
	xl = (double *)malloc(n*n*sizeof(double));
	eps = 1.0e-15;
	m = 0;
	d[0] = a[m];
	for(i = 1;i<n;i++)
	{
		for(j = 0;j<i;j++)
		{
			m = m+1;
			xl[i*n+j] = a[m]/d[j];
			if(j==0)
				continue;
			for(k = 0;k<j;k++)
			{
				xl[i*n+j] =xl[i*n+j]- xl[i*n+k]*xl[j*n+k]*d[k]/d[j];
			}
			m = m+1;
			d[i] = a[m];
			for(k = 0;k<i;k++)
			{
				d[i] = d[i] - d[k]*xl[i*n+k]*xl[i*n+k];
			}
			if(fabs(d[i])<eps)
				return;
		}
		y[0] = b[0];
		for(k = 1;k<n;k++)
		{
			y[k] = b[k];
			for(j = 0;j<k;j++)
			{
				y[k] = y[k] - xl[k*n+j]*y[j];
			}
		}
		b[n-1] = y[n-1]/d[n-1];
		for(k = (n-2);k>=0;k--)
		{
			b[k] = y[k]/d[k];
			for(j = (k+1);j<n;j++)
			{
				b[k] = b[k]-xl[j*n+k]*b[j];
			}
		}
	}
	free(d);
	free(y);
	free(xl);
}

//
void CSpectral::Pmlm(double wave[],int fs)
{
	mlm(wave,WindowLen,p,fs,psdData,dataFreq,PSD_LEN,1);
}
void CSpectral::mlm(double x[],int n,int pm,double fs,double s[],double freq[],int len,int sdb)
{
	int i,k,p;
	double v,ai,ar,im,re,zi,zr,sar,ang,tpi,*a;
	a = (double *)malloc((pm+1)*sizeof(double));
	tpi = 8.0*atan(1.0);
	for(i = 0;i<len;i++)
	{
		s[i] = 0.0;
	}
	for(p = 0;p<=pm;p++)
	{
		Burg(x,n,p,a,&v);
		for(k=0;k<len;k++)
		{
			ang = k*0.5/(len-1);
			freq[k] = ang*fs;
			zr = cos(-tpi*ang);
			zi = sin(-tpi*ang);
			ar = 0.0;
			ai = 0.0;
			for(i = p;i>0;i--)
			{
				re = ar;
				im = ai;
				ar = (re+a[i])*zr-im*zi;
				ai = (re+a[i])*zi+im*zr;
			}
			ar = ar+1.0;
			sar = v/(fs*(ar*ar+ai*ai));
			s[k] = s[k] +1.0/sar;
		}
	}
	for(k = 0;k<len;k++)
	{
		s[k] = pm/s[k];
	}
	if(sdb == 1)
	{
		for(k = 0;k<len;k++)
		{
			s[k] = 10.0*log10(s[k]);
		}
	}
	free(a);
}

void CSpectral::Psd(double b[],double a[],int q,int p,double sigma2,double fs,double x[],double freq[],int len,int sign)
{
	int i,k;
	double ar,ai,br,bi,zr,zi,im,re,xre,xim;
	double ang,den,numr,numi,temp;
	for(k=0;k<len;k++)
	{
		ang=k*0.5/(len-1);
		freq[k]=ang*fs;
		zr=cos(-8.0*atan(1.0)*ang);
		zi=sin(-8.0*atan(1.0)*ang);
		br=0.0;
		bi=0.0;
		for(i=q;i>0;i--)
		{
			re=br;
			im=bi;
			br=(re+b[i])*zr-im*zi;
			bi=(re+b[i])*zi+im*zr;
		}
		ar=0.0;
		ai=0.0;
		for(i=p;i>0;i--)
		{
			re=ar;
			im=ai;
			ar=(re+a[i])*zr-im*zi;
			ai=(re+a[i])*zi+im*zr;
		}
		br=br+b[0];
		ar=ar+1.0;
		numr=ar*br+ai*bi;
		numi=ar*bi-ai*br;
		den=ar*ar+ai*ai;
		xre=numr/den;
		xim=numi/den;
		switch(sign)
		{
		case 0:
			{
				x[k]=xre*xre+xim*xim;
				x[k]=sigma2*x[k]/fs;
				break;
			}
		case 1:
			{
				temp=xre*xre+xim*xim;
				temp=sigma2*temp/fs;
				if(temp==0.0)temp=1.0e-20;
				x[k]=10.0*log10(temp);
			}
		}
	}
}


double CSpectral::selectPSDFeature(double fre_low,double fre_high)
{
	double psd_feature = 0;
	for(int i = 0;i<PSD_LEN;i++)
	{
		if((dataFreq[i]>=fre_low) &&(dataFreq[i]<=fre_high))
		{
			psd_feature +=psdData[i];
		}
	}
	return psd_feature;
}


/* ����Ϊ�ִ���������غ���*/
   

/* ����Ϊ����Ҷ�任��������غ���*/
// ���ٸ���Ҷ�任   
// data ����Ϊ (2 * 2^n), data ��żλΪʵ������, data ����λΪ��������   
// isInverse��ʾ�Ƿ�Ϊ��任   

double CSpectral::PreFFT(double wave[],int n,double fre_low,double fre_high)
{

	FFT(wave,n);
	double res = selectFFTFeature(wave,fre_low,fre_high,2000,LEVEL);
	return res;
}

void CSpectral::FFT(double * data, int n, bool isInverse)   
{   
    int mmax, m, j, step, i;   
    double temp;   
    double theta, sin_htheta, sin_theta, pwr, wr, wi, tempr, tempi;   
    n = 2 * (1 << n);   
    int nn = n >> 1;   
    // ����Ϊ1�ĸ���Ҷ�任, λ�ý�������   
    j = 1;   
    for(i = 1; i < n; i += 2)   
    {   
        if(j > i)   
        {   
            temp = data[j - 1];   
            data[j - 1] = data[i - 1];   
            data[i - 1] = temp;   
            data[j] = temp;   
            data[j] = data[i];   
            data[i] = temp;   
        }   
        // �෴�Ķ����Ƽӷ�   
        m = nn;   
        while(m >= 2 && j > m)   
        {   
            j -= m;   
            m >>= 1;   
        }   
        j += m;   
    }   
    // Danielson - Lanczos ����Ӧ��   
    mmax = 2;   
    while(n > mmax)   
    {   
        step = mmax << 1;   
        theta = DOUBLE_PI / mmax;   
        if(isInverse)   
        {   
            theta = -theta;   
        }   
        sin_htheta = sin(0.5 * theta);   
        sin_theta = sin(theta);   
        pwr = -2.0 * sin_htheta * sin_htheta;   
        wr = 1.0;   
        wi = 0.0;   
        for(m = 1; m < mmax; m += 2)   
        {   
            for(i = m; i <= n; i += step)   
            {   
                j = i + mmax;   
                tempr = wr * data[j - 1] - wi * data[j];   
                tempi = wr * data[j] + wi * data[j - 1];   
                data[j - 1] = data[i - 1] - tempr;   
                data[j] = data[i] - tempi;   
                data[i - 1] += tempr;   
                data[i] += tempi;   
            }   
            sin_htheta = wr;   
            wr = sin_htheta * pwr - wi * sin_theta + wr;   
            wi = wi * pwr + sin_htheta * sin_theta + wi;   
        }   
        mmax = step;   
    }   
} 
double CSpectral::selectFFTFeature(double wave[], double fre_low,double fre_high,int fs,int n)
{
	double lfp_psd_feature = 0;
	for(int i = (int)fre_low*n/fs;i<(int)fre_high*n/fs;i++)
	{

		lfp_psd_feature +=sqrt(wave[i*2]*wave[i*2]+wave[i*2+1]*wave[i*2+1]);
	}
	return lfp_psd_feature;
}

/* ����Ϊ����Ҷ�任��������غ���*/
