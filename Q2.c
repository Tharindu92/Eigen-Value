#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int mat[9] = {2,3,2,10,3,4,3,6,1};
double vec[3] = {0,0,1};
double vec2[3] = {0,0,1};
double ans = 11;

double* mat_page;
double* val;
double* vector;
int* col_indx;
int* row_ptr;

double power_iteration(int n, FILE *file, int* mat, double* vec, double ans);
double shifted_power_iteration(int n, FILE *file, int* mat, double* vec, double ans, int shift);
double get_max(int n, double* vec);
double get_abs(double num);
int createRandomArray(int n, double * mat);
void dense_to_csr_convertion(int n,double* mat, double* val, int* col_indx, int* row_ptr);
void pageRankAlgo(int n, double* vec, double* val, int* col_indx, int* row_ptr);
void init_vector(int n, double* vec);
void print_dense_to_csr_convertion(int n, double* val, int* col_indx, int* row_ptr);

int main() {
    int n = 3;
    int shift = -3;
    int choise;
    double ans2;
    int needed_size;
    printf("Press 1 for power iteration\n");
    printf("Press 2 for shifted power iteration\n");
    printf("Press anything for finding page rank\n");

    printf("Enter your preference ");
    scanf("%d",&choise);

    if(choise == 1){
        FILE *f = fopen("power_iteration.txt", "w");
        ans2 = power_iteration(n,f,mat,vec,ans);
        printf("%lf\n",ans2);
        fclose(f);
    }else if(choise == 2){
        FILE *f1 = fopen("shifted_power_iteration.txt","w");
        ans2 = shifted_power_iteration(n,f1,mat,vec2,ans,shift);
        printf("%lf\n",ans2);
        fclose(f1);
    }else{
        n = 1000;
        /*
         * allocate memory
         */
        mat_page = calloc(n*n, sizeof(double));
        needed_size = createRandomArray(n,mat_page);
        val = malloc(needed_size*sizeof(double));
        col_indx = malloc(needed_size*sizeof(int));
        row_ptr = malloc((n+1)*sizeof(int));
        vector = malloc(n*sizeof(double));

        init_vector(n,vector);
        dense_to_csr_convertion(n,mat_page,val,col_indx,row_ptr);
        print_dense_to_csr_convertion(n,val,col_indx,row_ptr);
        printf("Done CSR conversion\n");
        pageRankAlgo(n,vector,val,col_indx,row_ptr);
        printf("Done\n");

        /*
         * free memory
         */
        free(mat_page);
        free(val);
        free(col_indx);
        free(row_ptr);
        free(vector);
    }
    return 0;
}

/*
 * Answer for Q2) b
 */
double power_iteration(int n, FILE *file, int* mat, double* vec, double ans){
    int i,j;
    double cal_ans = 1;
    double diff;
    double *temp_vec = malloc(n*sizeof(double));
    double temp;
    int k = 0;
    diff = get_abs(ans - cal_ans);
    fprintf(file,"\tk\t|\t\tXk\t\t\t|\tYk\n");
    fprintf(file, "\t%d\t|\t",k);
    for(i=0; i<n; i++){
        fprintf(file, "%.2f ",vec[i]);
    }
    fprintf(file, "\t|\t%lf\n",cal_ans);
    do{
        k++;
        for(i=0; i < n; i++){
            temp = 0;
            for(j=0 ; j<n; j++){
                temp += mat[i*n + j] * vec[j];
            }
            temp_vec[i] = temp;
        }
        memcpy(vec,temp_vec,n*sizeof(double));
        cal_ans = get_max(n,vec);
        fprintf(file, "\t%d\t|\t",k);
        for(i=0; i<n; i++){
            vec[i] = vec[i] / cal_ans;
            fprintf(file, "%.2lf ",vec[i]);
        }
        fprintf(file, "\t|\t%lf\n",cal_ans);

        diff = get_abs(ans - cal_ans);
    }while(diff > 0.0000001);
    free(temp_vec);
    return cal_ans;
}

/*
 * Answer for Q2) c
 */
double shifted_power_iteration(int n, FILE *file, int* mat, double* vec, double ans, int shift){
    int i,j;
    double cal_ans = 1;
    double diff;
    double *temp_vec = malloc(n*sizeof(double));
    double temp;
    int k = 0;
    diff = get_abs(ans - cal_ans);
    fprintf(file,"\tk\t|\t\tXk\t\t\t|\tYk\n");
    fprintf(file, "\t%d\t|\t",k);
    for(i=0; i<n; i++){
        fprintf(file, "%.2lf ",vec[i]);
    }
    fprintf(file, "\t|\t%lf\n",cal_ans);
    do{
        k++;
        for(i=0; i < n; i++){
            temp = 0;
            for(j=0 ; j<n; j++){
                if(i == j)
                    temp += (mat[i*n + j] - shift) * vec[j];
                else
                    temp += (mat[i*n + j]) * vec[j];
            }
            temp_vec[i] = temp;
        }
        memcpy(vec,temp_vec,n*sizeof(double));
        cal_ans = get_max(n,vec) + shift;
        fprintf(file, "\t%d\t|\t",k);
        for(i=0; i<n; i++){
            vec[i] = vec[i] / (cal_ans - shift);
            fprintf(file, "%.2f ",vec[i]);
        }
        fprintf(file, "\t|\t%lf\n",cal_ans);

        diff = get_abs(ans - cal_ans);

    }while(diff > 0.0000001);
    free(temp_vec);
    return cal_ans;
}

/*
 * Answer for Q2) e.
 */
void pageRankAlgo(int n, double* vec, double* val, int* col_indx, int* row_ptr){
    int i,j,nnz,temp2;
    double cal_ans = 1;
    double diff = 0.0;
    double *temp_vec = malloc(n*sizeof(double));
    double *temp_vec2 = malloc(n*sizeof(double));
    int *page_index = malloc(n*sizeof(int));

    double temp;
    int k = 0;
    /*
     * Run pageRank algorithm
     */
    do{
        k++;
        diff =0;
        nnz = 0;
        for(i=0; i < n; i++){
            temp = 0;
            for(j= row_ptr[i]; j<row_ptr[i+1]; j++){
                temp += val[nnz] * vec[col_indx[nnz] - 1];
                nnz++;
            }
            temp_vec[i] = temp;
            diff += get_abs(temp_vec[i] - temp_vec2[i]);
        }
        memcpy(vec,temp_vec,n*sizeof(double));
        memcpy(temp_vec2,temp_vec,n*sizeof(double));

        cal_ans = get_max(n,vec);
        for(i=0; i<n; i++){
            vec[i] = vec[i] / cal_ans;
        }
    }while(diff > 0.00000001);
    for(i = 0; i<n; i++)
        page_index[i] = i+1;

    /*
     * find page rank order
     */
    for(i = 0; i < n-i-1; i++){
        for(j = 0; j < n-i-1; j++){
            temp2 = page_index[i];
            if(temp_vec2[j] > temp_vec2[j+1]){
                page_index[j] = page_index[j+1];
                page_index[j+1] = temp2;
            }
        }
    }
    printf("Page rank : ");
    for(i = 0; i<n; i++)
        printf("%d ", page_index[i]);

    free(temp_vec);
    free(temp_vec2);
    free(page_index);
    printf("\niterations = %d\n",k);
}

int createRandomArray(int n, double* mat){
    int i = 0,j=0;
    int nnz,tot_nnz =0;
    int row;
    double temp;
    srand(time(NULL));
    while(i<n){
        nnz = rand()%16 + 5;
        j=0;
        while(j < nnz){
            row = rand()%n;
            mat[row*n + i] = (double)1.0/nnz;
            temp = mat[row*n + i];
            tot_nnz++;
            j++;
        }
        i++;
    }
    return tot_nnz;
}

void init_vector(int n, double* vec){
    int i;
    for(i = 0; i<n; i++){
        vec[i] = (double)1.0/n;
    }
}

void dense_to_csr_convertion(int n,double* mat, double* val, int* col_indx, int* row_ptr){
    int i,j;
    int nnz = 0;
    for(i = 0; i < n; i++){
        row_ptr[i] = nnz + 1;
        for(j = 0; j < n; j++){
            if(mat[i*n + j] != 0){
                *(val+nnz) = mat[i*n + j];
                *(col_indx + nnz) = j+1;
                nnz++;
            }
        }
    }
    row_ptr[n] = nnz + 1;
}

void print_dense_to_csr_convertion(int n, double* val, int* col_indx, int* row_ptr){
    int i;
    printf("Value    : ");
    for(i=0;i<n*n;i++){
        if(val[i] != 0)
            printf("%lf ",val[i]);
        else
            break;
    }
    printf("\nCol Index: ");
    for(i=0;i<n*n;i++){
        if(col_indx[i] != 0)
            printf("%d ",col_indx[i]);
        else
            break;
    }
    printf("\nPtr Index: ");
    for(i=0; i<=n ;i++){
        printf("%d ",row_ptr[i]);
    }
    printf("\nDone\n");
}

double get_max(int n, double* vec){
    int i;
    double max;

    for(i = 1; i<n;i++){
        if(max < get_abs(vec[i]))
            max = get_abs(vec[i]);
    }
    return max;
}

double get_abs(double num){
    if(num >= 0)
        return num;
    else
        return -num;
}

