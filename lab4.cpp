#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

using namespace std;

const int N = 30;
const int M = 3;

sem_t patient;  // 等候就诊患者
sem_t doctor;   // 医生人数
sem_t mutex, mutex1, mutex2;  // 互斥信号量，控制互斥访问PatientSum、PState、DState

int PatientSum = 0;  // 打算就诊、申请挂号的患者总数
string DState[M] = {"rest", "rest", "rest"};  // 医生状态：工作、休息
string PState[N];  // 患者状态：候诊、就医

void *Patients(void *arg) {
    int i = *(int *)arg;
    sem_wait(&mutex);
    if (PatientSum < N) {//判断是否有号，有号则候诊并修改自身状态，否则离开
        PatientSum++;
    } else {
        cout << "leave away!" << endl;
        exit(0);
    }
    sem_post(&mutex);
    sem_wait(&mutex1);
    PState[i] = "Waiting";
    sem_post(&mutex1);
    sem_post(&patient);//挂号成功，通知医生，有等待患者
    sem_wait(&doctor);//请求/等待医生为自己看病。如果有休息医生，将被signal(doctor)唤醒
    sem_wait(&mutex1); //轮到自己就诊，就诊前修改自身状态，就诊后离开
    PState[i] = "Treating";
    sem_post(&mutex1);
    cout << "Patient" << i << "Successfully Seeing the doctor!" << endl;
    sem_wait(&mutex1); //离开前还可以通过wait/signal(mutex1)将Pstate改为null
    PState[i] = nullptr;
    sem_post(&mutex1);
    pthread_exit(nullptr);
}

void *Doctor(void *arg) {
    int j = *(int *)arg;
    while (true) {
        sem_wait(&patient);/*如果无候诊患者，则休息
        如果有患者，将被patient发出的signal(patient)唤醒*/
        sem_wait(&mutex2);
        DState[j] = "Working";//修改自身工作状态
        sem_post(&mutex2);
        sem_post(&doctor);//医生召唤1个就诊患者
        cout << "Doctor " << j << " Seeing a patient" << endl;
        sem_wait(&mutex2);
        DState[j] = "rest";//看完病，转为休息状态
        sem_post(&mutex2);
    }
}

int main() {
    pthread_t patients[N], doctors[M];

    // 初始化信号量
    sem_init(&patient, 0, 0);
    sem_init(&doctor, 0, M);
    sem_init(&mutex, 0, 1);
    sem_init(&mutex1, 0, 1);
    sem_init(&mutex2, 0, 1);

    // 创建医生线程
    for (int i = 0; i < M; i++) {
        if (pthread_create(&doctors[i], nullptr, Doctor, (void *)&i) != 0) {
            perror("Failed to create a doctor thread");
            exit(EXIT_FAILURE);
        }
    }

    // 创建患者线程
    for (int i = 0; i < 30; i++) {
        if (pthread_create(&patients[i], nullptr, Patients, (void *)&i) != 0) {
            perror("Failed to create a patient thread");
            exit(EXIT_FAILURE);
        }
        // 模拟患者到达的时间间隔
        usleep(500000);
    }

    // 等待线程结束
    for (auto &p : patients) {
        pthread_join(p, nullptr);
    }

    for (auto &d : doctors) {
        pthread_join(d, nullptr);
    }

    // 销毁信号量
    sem_destroy(&patient);
    sem_destroy(&doctor);
    sem_destroy(&mutex);
    sem_destroy(&mutex1);
    sem_destroy(&mutex2);

    return 0;
}
