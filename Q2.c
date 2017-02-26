#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mat[9] = {2,3,2,10,3,4,3,6,1};
float vec[3] = {0,0,1};
float vec2[3] = {0,0,1};
float ans = 11;

float power_iteration(int n, FILE *file, int* mat, float* vec, float ans);
float shifted_power_iteration(int n, FILE *file, int* mat, float* vec, float ans, int shift);
float get_max(int n, float* vec);
float get_abs(float num);

int main() {
    int n = 3;
    int shift = 3;
    FILE *f = fopen("power_iteration.txt", "a");
    FILE *f1 = fopen("shifted_power_iteration.txt","a");
    float ans2 = power_iteration(n,f,mat,vec,ans);
    ans2 = shifted_power_iteration(n,f1,mat,vec2,ans,shift);
    printf("%f\n",ans2);
    fclose(f);
    return 0;
}

float power_iteration(int n, FILE *file, int* mat, float* vec, float ans){
    int i,j;
    float cal_ans = 1;
    float diff;
    float *temp_vec = malloc(n*sizeof(int));
    float temp;
    float temp1,temp2,temp3;
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
        temp1 = temp_vec[0];
        temp2 = temp_vec[1];
        temp3 = temp_vec[2];
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
    float temp1,temp2,temp3;
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
                temp += (mat[i*n + j] - shift) * vec[j];
            }
            temp_vec[i] = temp;
        }
        temp1 = temp_vec[0];
        temp2 = temp_vec[1];
        temp3 = temp_vec[2];
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

