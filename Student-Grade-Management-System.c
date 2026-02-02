#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 不再使用固定的宏定义
// #define MAX_STUDENTS 10
// #define MAX_SUBJECTS 3

// 学生信息结构体（动态版本）
struct Student {
    int id;
    char name[50];
    float *scores;      // 改为指针，动态分配内存
    float average;
    int numSubjects;    // 添加课程数量字段
};

// 函数声明
int inputStudentCount();
int inputSubjectCount();
struct Student* createStudents(int numStudents, int numSubjects);
void inputStudentInfo(struct Student students[], int numStudents, int numSubjects);
void calculateStudentAverage(struct Student students[], int numStudents);
void calculateSubjectAverage(struct Student students[], int numStudents, int numSubjects);
void findMaxScore(struct Student students[], int numStudents, int numSubjects);
void displayAllInfo(struct Student students[], int numStudents, int numSubjects);
void displayTopStudent(struct Student students[], int numStudents);
void freeStudents(struct Student students[], int numStudents);

int main() {
    int numStudents, numSubjects;
    struct Student *students;
    
    printf("========== 学生成绩管理系统（动态版本）==========\n");
    
    // 输入学生数量
    numStudents = inputStudentCount();
    
    // 输入课程数量
    numSubjects = inputSubjectCount();
    
    // 动态创建学生数组
    students = createStudents(numStudents, numSubjects);
    if (students == NULL) {
        printf("内存分配失败！\n");
        return 1;
    }
    
    // 输入学生信息
    inputStudentInfo(students, numStudents, numSubjects);
    
    // 计算学生平均分
    calculateStudentAverage(students, numStudents);
    
    // 显示所有信息
    displayAllInfo(students, numStudents, numSubjects);
    
    // 计算并显示科目平均分
    calculateSubjectAverage(students, numStudents, numSubjects);
    
    // 查找最高分
    findMaxScore(students, numStudents, numSubjects);
    
    // 显示最优秀学生
    displayTopStudent(students, numStudents);
    
    // 释放内存
    freeStudents(students, numStudents);
    
    printf("\n========== 程序执行完毕 ==========\n");
    return 0;
}

// 输入学生数量
int inputStudentCount() {
    int count;
    while (1) {
        printf("\n请输入学生人数: ");
        if (scanf("%d", &count) != 1 || count <= 0) {
            printf("输入错误！请输入一个正整数。\n");
            while (getchar() != '\n'); // 清除错误输入
        } else {
            break;
        }
    }
    return count;
}

// 输入课程数量
int inputSubjectCount() {
    int count;
    while (1) {
        printf("请输入课程数量: ");
        if (scanf("%d", &count) != 1 || count <= 0) {
            printf("输入错误！请输入一个正整数。\n");
            while (getchar() != '\n'); // 清除错误输入
        } else {
            break;
        }
    }
    return count;
}

// 动态创建学生数组
struct Student* createStudents(int numStudents, int numSubjects) {
    struct Student *students = (struct Student*)malloc(numStudents * sizeof(struct Student));
    if (students == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < numStudents; i++) {
        students[i].scores = (float*)malloc(numSubjects * sizeof(float));
        if (students[i].scores == NULL) {
            // 如果分配失败，释放已分配的内存
            for (int j = 0; j < i; j++) {
                free(students[j].scores);
            }
            free(students);
            return NULL;
        }
        students[i].numSubjects = numSubjects;
    }
    
    return students;
}

// 输入学生信息（动态版本）
void inputStudentInfo(struct Student students[], int numStudents, int numSubjects) {
    printf("\n---------- 输入学生信息 ----------\n");
    for (int i = 0; i < numStudents; i++) {
        printf("\n学生 #%d:\n", i + 1);
        
        printf("学号: ");
        scanf("%d", &students[i].id);
        while (getchar() != '\n'); // 清除输入缓冲区
        
        printf("姓名: ");
        fgets(students[i].name, sizeof(students[i].name), stdin);
        students[i].name[strcspn(students[i].name, "\n")] = '\0';
        
        printf("请输入 %d 门课程的成绩:\n", numSubjects);
        for (int j = 0; j < numSubjects; j++) {
            printf("  课程 %d: ", j + 1);
            scanf("%f", &students[i].scores[j]);
        }
        while (getchar() != '\n');
    }
}

// 计算每个学生的平均分（动态版本）
void calculateStudentAverage(struct Student students[], int numStudents) {
    for (int i = 0; i < numStudents; i++) {
        float sum = 0;
        int numSubjects = students[i].numSubjects;
        
        for (int j = 0; j < numSubjects; j++) {
            sum += students[i].scores[j];
        }
        students[i].average = sum / numSubjects;
    }
}

// 计算每门课程的平均分（动态版本）
void calculateSubjectAverage(struct Student students[], int numStudents, int numSubjects) {
    printf("\n---------- 科目平均分 ----------\n");
    
    float *subjectSum = (float*)calloc(numSubjects, sizeof(float));
    if (subjectSum == NULL) {
        printf("内存不足，无法计算科目平均分！\n");
        return;
    }
    
    // 计算每门课程的总分
    for (int i = 0; i < numStudents; i++) {
        for (int j = 0; j < numSubjects; j++) {
            subjectSum[j] += students[i].scores[j];
        }
    }
    
    // 计算并显示平均分
    for (int j = 0; j < numSubjects; j++) {
        float avg = subjectSum[j] / numStudents;
        printf("课程 %d 平均分: %.2f\n", j + 1, avg);
    }
    
    free(subjectSum);
}

// 查找最高分（动态版本）
void findMaxScore(struct Student students[], int numStudents, int numSubjects) {
    if (numStudents == 0 || numSubjects == 0) return;
    
    float maxScore = students[0].scores[0];
    int maxStudent = 0, maxSubject = 0;
    
    for (int i = 0; i < numStudents; i++) {
        for (int j = 0; j < numSubjects; j++) {
            if (students[i].scores[j] > maxScore) {
                maxScore = students[i].scores[j];
                maxStudent = i;
                maxSubject = j;
            }
        }
    }
    
    printf("\n---------- 最高分信息 ----------\n");
    printf("最高分: %.2f\n", maxScore);
    printf("获得者: %s (学号: %d)\n", students[maxStudent].name, students[maxStudent].id);
    printf("课程: 第 %d 门课\n", maxSubject + 1);
}

// 显示所有信息（动态版本）
void displayAllInfo(struct Student students[], int numStudents, int numSubjects) {
    printf("\n---------- 学生成绩总览 ----------\n");
    
    for (int i = 0; i < numStudents; i++) {
        printf("\n学生 #%d:\n", i + 1);
        printf("  学号: %d\n", students[i].id);
        printf("  姓名: %s\n", students[i].name);
        printf("  成绩: ");
        
        for (int j = 0; j < numSubjects; j++) {
            printf("课程%d: %.2f  ", j + 1, students[i].scores[j]);
        }
        
        printf("\n  平均分: %.2f\n", students[i].average);
    }
}

// 显示最优秀学生（动态版本）
void displayTopStudent(struct Student students[], int numStudents) {
    if (numStudents == 0) return;
    
    int topIndex = 0;
    float maxAverage = students[0].average;
    
    for (int i = 1; i < numStudents; i++) {
        if (students[i].average > maxAverage) {
            maxAverage = students[i].average;
            topIndex = i;
        }
    }
    
    int numSubjects = students[topIndex].numSubjects;
    
    printf("\n---------- 最优秀学生 ----------\n");
    printf("姓名: %s\n", students[topIndex].name);
    printf("学号: %d\n", students[topIndex].id);
    printf("平均分: %.2f\n", students[topIndex].average);
    printf("各科成绩:\n");
    
    for (int j = 0; j < numSubjects; j++) {
        printf("  课程 %d: %.2f\n", j + 1, students[topIndex].scores[j]);
    }
}

// 释放内存
void freeStudents(struct Student students[], int numStudents) {
    for (int i = 0; i < numStudents; i++) {
        free(students[i].scores);
    }
    free(students);
}