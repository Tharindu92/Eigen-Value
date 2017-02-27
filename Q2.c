#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int mat[9] = {2,3,2,10,3,4,3,6,1};
float vec[3] = {0,0,1};
float vec2[3] = {0,0,1};
float ans = 11;

double* mat_page;
double* val;
int* col_indx;
int* row_ptr;

float power_iteration(int n, FILE *file, int* mat, float* vec, float ans);
float shifted_power_iteration(int n, FILE *file, int* mat, float* vec, float ans, int shift);
float get_max(int n, float* vec);
float get_abs(float num);
int createRandomArray(int n, int* mat);
void dense_to_csr_convertion(int n,int* mat, int* val, int* col_indx, int* row_ptr);

int main() {
    int n = 3;
    int shift = -3;
    int choise;
    float ans2;
    int needed_size;
    FILE *f = fopen("power_iteration.txt", "a");
    FILE *f1 = fopen("shifted_power_iteration.txt","a");
    printf("Press 1 for power iteration\n");
    printf("Press 2 for shifted power iteration\n");
    printf("Press anything for finding page rank\n");

    printf("Enter your preference ");
    scanf("%d",&choise);

    if(choise == 1){
        ans2 = power_iteration(n,f,mat,vec,ans);
        printf("%f\n",ans2);
    }else if(choise == 2){
        ans2 = shifted_power_iteration(n,f1,mat,vec2,ans,shift);
        printf("%f\n",ans2);
    }else{
        n = 1000;
        mat_page = calloc(n*n, sizeof(double));
        needed_size = createRandomArray(n,mat_page);
        val = malloc(needed_size*sizeof(double));
        col_indx = malloc(needed_size*sizeof(int));
        row_ptr = malloc((n+1)*sizeof(int));

        dense_to_csr_convertion(n,mat,val,col_indx,row_ptr);
    }
    fclose(f);
    return 0;
}

float power_iteration(int n, FILE *file, int* mat, float* vec, float ans){
    int i,j;
    float cal_ans = 1;
    float diff;
    float *temp_vec = malloc(n*sizeof(int));
    float temp;
    int k = 0;
    if(cal_ans > ans)
        diff = cal_ans - ans;
    else
        diff = ans - cal_ans;
    fprintf(file,"\tk\t|\t\tXk\t\t\t|\tYk\n");
    fprintf(file, "\t%d\t|\t",k);
    for(i=0; i<n; i++){
        fprintf(file, "%.2f ",vec[i]);
    }
    fprintf(file, "\t|\t%f\n",cal_ans);
    do{
        k++;
        for(i=0; i < n; i++){
            temp = 0;
            for(j=0 ; j<n; j++){
                temp += mat[i*n + j] * vec[j];
            }
            temp_vec[i] = temp;
        }
        memcpy(vec,temp_vec,n*sizeof(int));
        cal_ans = get_max(n,vec);
        fprintf(file, "\t%d\t|\t",k);
        for(i=0; i<n; i++){
            vec[i] = vec[i] / cal_ans;
            fprintf(file, "%.2f ",vec[i]);
        }
        fprintf(file, "\t|\t%f\n",cal_ans);

        if(cal_ans > ans)
            diff = cal_ans - ans;
        else
            diff = ans - cal_ans;

    }while(diff > 0);
    return cal_ans;
}

float shifted_power_iteration(int n, FILE *file, int* mat, float* vec, float ans, int shift){
    int i,j;
    float cal_ans = 1;
    float diff;
    float *temp_vec = malloc(n*sizeof(int));
    float temp;
    int k = 0;
    if(cal_ans > ans)
        diff = cal_ans - ans;
    else
        diff = ans - cal_ans;
    fprintf(file,"\tk\t|\t\tXk\t\t\t|\tYk\n");
    fprintf(file, "\t%d\t|\t",k);
    for(i=0; i<n; i++){
        fprintf(file, "%.2f ",vec[i]);
    }
    fprintf(file, "\t|\t%f\n",cal_ans);
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
        memcpy(vec,temp_vec,n*sizeof(int));
        cal_ans = get_max(n,vec) + shift;
        fprintf(file, "\t%d\t|\t",k);
        for(i=0; i<n; i++){
            vec[i] = vec[i] / (cal_ans - shift);
            fprintf(file, "%.2f ",vec[i]);
        }
        fprintf(file, "\t|\t%f\n",cal_ans);

        if(cal_ans > ans)
            diff = cal_ans - ans;
        else
            diff = ans - cal_ans;

    }while(diff > 0);
    return cal_ans;
}

int createRandomArray(int n, int* mat){
    int i = 0,j=0;
    int nnz,tot_nnz =0;
    int row;
    srand(time(NULL));
    while(i<n){
        nnz = rand()%16 + 5;
        j=0;
        while(j < nnz){
            row = rand()%n;
            mat[row*n + i] = 1/nnz;
            tot_nnz++;
            j++;
        }
    }
    return tot_nnz;
}

void dense_to_csr_convertion(int n,int* mat, int* val, int* col_indx, int* row_ptr){
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

float get_max(int n, float* vec){
    int i;
    float max;

    for(i = 1; i<n;i++){
        if(max < get_abs(vec[i]))
            max = get_abs(vec[i]);
    }
    return max;
}

float get_abs(float num){
    if(num >= 0)
        return num;
    else
        return -num;
}

