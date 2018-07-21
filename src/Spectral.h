#define ORDER 5
#define PSD_LEN 10000
#define LEVEL 512
#define DOUBLE_PI 6.283

class CSpectral
{
public:
	CSpectral(int winlen);
	~CSpectral(void);

    //burg
	void Pburg(double wave[],int fs);
	void Burg(double x[],int n,int p,double a[],double *v);
	
	//yulewalk
	void Pyulear(double wave[],int fs);
	void yulewalk(double x[],int n,int p,double a[],double *v);
	void levinson(double a[],double r[],int p,double *v);

	// covar
	void Pcovar(double wave[],int fs,int mode);
	void covar(double x[],int n, int p,double a[],double *v, int mode);
	void cholesky(double a[],double b[],int n);

	// mlm
	void Pmlm(double wave[],int fs);
	void mlm(double x[],int n,int pm,double fs,double s[],double freq[],int len,int sdb);

    //
	void Psd(double b[],double a[],int q,int p,double sigma2,double fs,double x[],double freq[],int len,int sign);
	double selectPSDFeature(double fre_low,double fre_high);


	// fft
	void FFT(double * data, int n, bool isInverse = false);
	double PreFFT(double wave[],int n,double fre_low,double fre_high);
	double selectFFTFeature(double wave[], double fre_low,double fre_high,int fs,int n);

private:

	double *psdData;
	double *dataFreq;
	double *dataFFT;
	int p;
	void InitArray();
public:
	int WindowLen;
};

