double totalTimeCalculation(int n, int n_dim);
double timeStepCalculation(int n, double epsilon, int n_dim);
void forceCalculation(double *p, double *v, double *a, int n, double epsilon, int n_dim);
void  kick(double *p, double *v, double *a, int n, double delta_t, int n_dim);
void  drift(double *p, double *v, double *a, int n, double delta_t, int n_dim);
void energyCalculation(double *p, double *v, double *U, double *K, int n, int n_dim);
