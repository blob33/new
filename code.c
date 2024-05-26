// 1.Synchronization
// a. Readers-writers
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
sem_t mutex, writeblock;
int data = 0, rcount = 0;
void *reader(void *arg)
{
    int f;
    f = *((int *)arg);
    sem_wait(&mutex);
    rcount++;
    if (rcount == 1)
    {
        sem_wait(&writeblock);
    }
    sem_post(&mutex);
    printf("Reader %d is reading data: %d\n", f, data);
    sleep(2);
    sem_wait(&mutex);
    rcount--;
    if (rcount == 0)
    {
        sem_post(&writeblock);
    }
    sem_post(&mutex);
    return NULL;
}
void *writer(void *arg)
{
    int f;
    f = *((int *)arg);
    sem_wait(&writeblock);
    data++;
    printf("Data Written By The Writer: %d\n", data);
    sleep(1);
    sem_post(&writeblock);
    return NULL;
}
int main()
{
    int i;
    pthread_t rtid[3], wtid[3];
    sem_init(&mutex, 0, 1);
    sem_init(&writeblock, 0, 1);
    while (1)
    {
        for (i = 0; i < 3; i++)
        {
            pthread_create(&wtid[i], NULL, writer, &i);
            pthread_create(&rtid[i], NULL, reader, &i);
        }
        for (i = 0; i < 3; i++)
        {
            pthread_join(wtid[i], NULL);
            pthread_join(rtid[i], NULL);
        }
        sleep(5); // Delay for 5 seconds before running the loop again
        printf("\n\n");
    }
    return 0;
}

//Java
import java.util.concurrent.Semaphore;

public class Main {
  
  static Semaphore readLock = new Semaphore(1);
  static Semaphore writeLock = new Semaphore(1);
  static int readCount=0;
  
  static class Read implements Runnable{
    
    public void run(){
      try{
        
        readLock.acquire();
        readCount++;
        if(readCount==0){
          writeLock.acquire();
        }
        readLock.release();
        
        System.out.println("Thread " + Thread.currentThread().getName() +" is Reading");
        Thread.sleep(1500);
        System.out.println("Thread "+Thread.currentThread().getName() + " is finishing reading.");
        
        readLock.acquire();
        readCount--;
        if(readCount==0){
          writeLock.release();
        }
        readLock.release();
      }catch(Exception e){
        System.out.println(e);
      }  
    }
  }
  
  static class Write implements Runnable{
    public void run(){
      try{
        writeLock.acquire();
        System.out.println("Thread " + Thread.currentThread().getName() +" is Writing ");
        Thread.sleep(1500);
        System.out.println("Thread " + Thread.currentThread().getName() +" is finishing writing.");
        writeLock.release();
      }catch(Exception e){
        System.out.println(e);
      }
    }
  }
  
    public static void main(String[] args) {
      
      Read read = new Read();
      Write write = new Write();
      
      Thread t1 = new Thread(read);
      t1.setName("thread1");
      Thread t2 = new Thread(read);
      t2.setName("thread2");
      Thread t3 = new Thread(write);
      t3.setName("thread3");
      Thread t4 = new Thread(read);
      t4.setName("thread4");
      
      t1.start();
      t2.start();
      t3.start();
      t4.start();
      
  }
}

// b. Producer-consumer

// c.Dining-philosopher
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

sem_t room;
sem_t chopstick[5];

void * philosopher(void *);
void eat(int);
int main()
{
	int i,a[5];
	pthread_t tid[5];
	
	sem_init(&room,0,4);
	
	for(i=0;i<5;i++)
		sem_init(&chopstick[i],0,1);
		
	for(i=0;i<5;i++){
		a[i]=i;
		pthread_create(&tid[i],NULL,philosopher,(void *)&a[i]);
	}
	for(i=0;i<5;i++)
		pthread_join(tid[i],NULL);
}

void * philosopher(void * num){
	int phil=*(int *)num;

	sem_wait(&room);
	printf("\nPhilosopher %d has entered room",phil);
	sem_wait(&chopstick[phil]);
	sem_wait(&chopstick[(phil+1)%5]);

	eat(phil);
	sleep(2);
	printf("\nPhilosopher %d has finished eating",phil);

	sem_post(&chopstick[(phil+1)%5]);
	sem_post(&chopstick[phil]);
	sem_post(&room);
}

void eat(int phil){
	printf("\nPhilosopher %d is eating",phil);
}

//Java
import java.util.*;
import java.util.concurrent.Semaphore;

public class Main {
  
  public static final int philosophers = 5;
  public static final Semaphore[] forks = new Semaphore[philosophers];
  public static final Semaphore dineSema = new Semaphore(philosophers-1);
  
    public static void main(String[] args) {
      
      for(int i=0; i<philosophers;i++){
        forks[i] = new Semaphore(1);
      }
      
      Thread[] philo = new Thread[philosophers];
      for(int i=0;i<philosophers;i++){
        final int id = i;
        philo[i] = new Thread(()-> dine(id));
        philo[i].start();
      }
      
      try{
        for(Thread philos: philo){
          philos.join();
        }
      }catch(Exception e){
        System.out.println(e);
      }
  }
  
  static void dine(int philoId){
    while(true){
      think(philoId);
      
      try{
        dineSema.acquire();
        forks[philoId].acquire();
        forks[(philoId+1)%philosophers].acquire();
        
        eat(philoId);
        
        forks[(philoId+1)%philosophers].release();
        forks[philoId].release();
        dineSema.release();
      }catch(Exception e){
        System.out.println(e);
      }
      
    }
  }
  
  public static void think(int id){
    System.out.println("philosopher "+ id+" is thinking");
    try {
            Thread.sleep((long) (Math.random() * 1000));
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
  }
  
  public static void eat(int id){
    System.out.println("philosopher "+ id+" is eating");
    try {
            Thread.sleep((long) (Math.random() * 1000));
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
  }
  
}


// 2. Scheduling Algorithms
// a.FCFS
#include <stdio.h>

struct Process{
  int id;
  int AT;
  int BT;
  int WT;
  int TT;
  int completed;
};

void fcfs(struct Process processes[], int n){
  
  int current=0;
  float totalWT = 0, totalTT=0;
  
  for(int i=0;i<n;i++){
    
    processes[i].WT = current-processes[i].BT;
    if(processes[i].WT<0){
      processes[i].WT = 0;
    }
    
    totalWT += processes[i].WT;
    
    processes[i].TT = processes[i].WT+processes[i].BT;
    totalTT += processes[i].TT;
    
    current += processes[i].BT;
    
  }
  
  printf("Process AT \tBT \tWT \tTT\n");
  for(int i=0;i<n;i++){
    printf("%d\t %d\t %d\t %d\t %d\t\n", processes[i].id, processes[i].AT, processes[i].BT, processes[i].WT, processes[i].TT);
  }
  
}

int main()
{
    struct Process processes[]={
      {0, 0, 5, 0, 0, 0},
      {1, 1, 3, 0, 0, 0},
      {2, 2, 8, 0, 0, 0},
      {3, 3, 6, 0, 0, 0}
    };
    
    int n= 4;
    fcfs(processes, n);
    
}

// b.Priority(Preempt)
#include <stdio.h>

struct Process {
    int id;
    int AT;
    int BT;
    int P;
    int WT;
    int TT;
    int remaining;
    int done;
    int FT;
};

void priority_preemptive(struct Process pcs[], int n) {
    int current = 0, totalWT = 0, totalTT = 0;
    int completed = 0;

    for (int i = 0; i < n; i++) {
        pcs[i].remaining = pcs[i].BT;
        pcs[i].done = 0;
    }

    while (completed < n) {
        int highpriority = -1;

        for (int i = 0; i < n; i++) {
            if (pcs[i].AT <= current && pcs[i].remaining > 0 && !pcs[i].done) {
                if (highpriority == -1 || pcs[i].P < pcs[highpriority].P) {
                    highpriority = i;
                }
            }
        }

        if (highpriority == -1) {
            current++;
            continue;
        }

        pcs[highpriority].remaining--;

        if (pcs[highpriority].remaining == 0) {
            pcs[highpriority].FT = current + 1; // Finish time
            pcs[highpriority].WT = pcs[highpriority].FT - pcs[highpriority].AT - pcs[highpriority].BT; // Waiting time
            pcs[highpriority].TT = pcs[highpriority].FT - pcs[highpriority].AT; // Turnaround time
            totalWT += pcs[highpriority].WT;
            totalTT += pcs[highpriority].TT;
            pcs[highpriority].done = 1;
            completed++;
        }
        current++;
    }

    printf("Process\t Arrival Time\t Burst Time\t Priority\t Waiting Time\t Turnaround Time\t Finish Time\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", pcs[i].id, pcs[i].AT, pcs[i].BT, pcs[i].P, pcs[i].WT, pcs[i].TT, pcs[i].FT);
    }

    printf("\nAverage Waiting Time: %.2f\n", (float)totalWT / n);
    printf("Average Turnaround Time: %.2f\n", (float)totalTT / n);
}

int main() {
    struct Process processes[] = {
        {0, 0, 5, 1, 0, 0, 0, 0, 0},
        {1, 1, 3, 2, 0, 0, 0, 0, 0},
        {2, 2, 8, 1, 0, 0, 0, 0, 0},
        {3, 3, 6, 3, 0, 0, 0, 0, 0}
    };
    int n = 4;

    priority_preemptive(processes, n);

    return 0;
}

// c.Priority(Non-preempt)
#include <stdio.h>

struct Process {
    int id;
    int AT;
    int BT;
    int pri;
    int WT;
    int TT;
    int done;
};

void priority(struct Process pcs[], int size){
  
  int current=0, completed=0;
  float totalWT=0, totalTT=0;
  
  while(completed<size){
  int highpriority=-1;
  for(int i=0;i<size;i++){
    if(pcs[i].AT <= current && !pcs[i].done){
      highpriority = i;
    }
  }
  
  pcs[highpriority].WT = current - pcs[highpriority].AT;
  totalWT+=pcs[highpriority].WT;
  
  pcs[highpriority].TT = pcs[highpriority].WT+pcs[highpriority].BT;
  totalTT += pcs[highpriority].TT;
  
  current += pcs[highpriority].BT;
  pcs[highpriority].done = 1;
  completed++;
  }
  
  printf("Process AT \tBT \tWT \tTT\n");
  for(int i=0;i<size;i++){
    printf("%d\t %d\t %d\t %d\t %d\t\n", pcs[i].id, pcs[i].AT, pcs[i].BT, pcs[i].WT, pcs[i].TT);
  }

}


int main()
{
    struct Process processes[] = {
        {0, 0, 5, 1, 0, 0, 0},
        {1, 1, 3, 2, 0, 0, 0},
        {2, 2, 8, 1, 0, 0, 0},
        {3, 3, 6, 3, 0, 0, 0}
    };
    int n = sizeof(processes) / sizeof(processes[0]);
    
    priority(processes, n);
    
}

// d.SJF(Non-Preempt)
#include<stdio.h>

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int completed;
};

void findWaitingTime(struct Process processes[], int n) {
    int currentTime = 0, completedProcesses = 0, shortest = 0;
    float total_waiting_time = 0, total_turnaround_time = 0, tt, wt;

    while (completedProcesses < n) {
        shortest = -1;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= currentTime && !processes[i].completed) {
                if (shortest == -1 || processes[i].burst_time < processes[shortest].burst_time) {
                    shortest = i;
                }
            }
        }

        if (shortest == -1) {
            currentTime++;
            continue;
        }

        processes[shortest].waiting_time = currentTime - processes[shortest].arrival_time;
        processes[shortest].turnaround_time = processes[shortest].waiting_time + processes[shortest].burst_time;

        total_waiting_time += processes[shortest].waiting_time;
        total_turnaround_time += processes[shortest].turnaround_time;

        currentTime += processes[shortest].burst_time;
        processes[shortest].completed = 1;
        completedProcesses++;
    }

    printf("Process\t Arrival Time\t Burst Time\t Waiting Time\t Turnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].arrival_time, processes[i].burst_time, processes[i].waiting_time, processes[i].turnaround_time);
    }
    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / n);
    printf("Average Waiting Time: %.2f\n", total_waiting_time / n);
}

int main() {
    struct Process processes[] = {
        {0, 0, 5, 0, 0, 0},
        {1, 1, 3, 0, 0, 0},
        {2, 2, 8, 0, 0, 0},
        {3, 3, 6, 0, 0, 0}
    };
    int n = sizeof(processes) / sizeof(processes[0]);

    findWaitingTime(processes, n);

    return 0;
}

// e.SRTF(Preemt)
#include<stdio.h>

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    int completed;
};

void findWaitingTime(struct Process processes[], int n) {
    int currentTime = 0, completedProcesses = 0;
    float total_waiting_time = 0, total_turnaround_time = 0;

    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
    }

    while (completedProcesses < n) {
        int shortest = -1;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= currentTime && processes[i].remaining_time > 0) {
                if (shortest == -1 || processes[i].remaining_time < processes[shortest].remaining_time) {
                    shortest = i;
                }
            }
        }

        if (shortest == -1) {
            currentTime++;
            continue;
        }

        processes[shortest].remaining_time--;

        if (processes[shortest].remaining_time == 0) {
            completedProcesses++;

            processes[shortest].waiting_time = currentTime + 1 - processes[shortest].arrival_time - processes[shortest].burst_time;
            processes[shortest].turnaround_time = processes[shortest].waiting_time + processes[shortest].burst_time;

            total_waiting_time += processes[shortest].waiting_time;
            total_turnaround_time += processes[shortest].turnaround_time;
        }

        currentTime++;
    }

    printf("Process\t Arrival Time\t Burst Time\t Waiting Time\t Turnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].arrival_time, processes[i].burst_time, processes[i].waiting_time, processes[i].turnaround_time);
    }
    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / n);
    printf("Average Waiting Time: %.2f\n", total_waiting_time / n);
}

int main() {
    struct Process processes[] = {
        {0, 0, 5, 0, 0, 0, 0},
        {1, 1, 3, 0, 0, 0, 0},
        {2, 2, 8, 0, 0, 0, 0},
        {3, 3, 6, 0, 0, 0, 0}
    };
    int n = sizeof(processes) / sizeof(processes[0]);

    findWaitingTime(processes, n);

    return 0;
}

// f.Round Robin(Non-preempt)
#include<stdio.h>

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    int completed;
};

void findWaitingTime(struct Process processes[], int n) {
    int currentTime = 0, completedProcesses = 0, quantum = 2, mi = 99;
    float total_waiting_time = 0, total_turnaround_time = 0;
    int queue[n];

    int front = 0, rear = 0;
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        if(mi > processes[i].arrival_time){
            mi = processes[i].arrival_time;
            queue[front] = i;
        }
    }

    while (completedProcesses < n) {
        int currentProcess = queue[front];

        if (processes[currentProcess].remaining_time > 0) {
            if (processes[currentProcess].remaining_time > quantum) {
                currentTime += quantum;
                processes[currentProcess].remaining_time -= quantum;

            } else {
                currentTime += processes[currentProcess].remaining_time;
                processes[currentProcess].waiting_time = currentTime - processes[currentProcess].arrival_time - processes[currentProcess].burst_time;
                processes[currentProcess].remaining_time = 0;
                processes[currentProcess].completed = 1;
                completedProcesses++;

                processes[currentProcess].turnaround_time = currentTime - processes[currentProcess].arrival_time;

                total_waiting_time += processes[currentProcess].waiting_time;
                total_turnaround_time += processes[currentProcess].turnaround_time;
            }

            for (int i = 0; i < n; i++) {
                if (!processes[i].completed && processes[i].arrival_time <= currentTime) {
                    rear = (rear + 1) % n;
                    queue[rear] = i;
                }
            }
            front = (front + 1) % n;
        }
    }

    printf("Process\t Arrival Time\t Burst Time\t Waiting Time\t Turnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].arrival_time, processes[i].burst_time, processes[i].waiting_time, processes[i].turnaround_time);
    }
    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / n);
    printf("Average Waiting Time: %.2f\n", total_waiting_time / n);
}

int main() {
    struct Process processes[] = {
        {0, 0, 5, 0, 0, 0, 0},
        {1, 1, 3, 0, 0, 0, 0},
        {2, 2, 8, 0, 0, 0, 0},
        {3, 3, 6, 0, 0, 0, 0}
    };
    int n = sizeof(processes) / sizeof(processes[0]);

    findWaitingTime(processes, n);

    return 0;
}

// g.Round Robin(Preempt)

// 3.Bankers Algorithm
#include <stdio.h>

int processes[5]={0,1,2,3,4};
int allocate[5][3]={
    {0,1,0},
    {2,0,0},
    {3,0,2},
    {2,1,2},
    {0,0,2}
};

int max[5][3]={
    {7,5,3},
    {3,2,2},
    {9,0,2},
    {2,2,2},
    {4,3,3}
};

int work[3] = {3,3,2};
int need[5][3];
int finish[5];
int sequence[5];
int m = 3;
int n = 5;
int index=0;

void bankers(){
  
  for(int i=0;i<2;i++){
    for(int j=0;j<n;j++){
      if(finish[j]==0){
        if( need[j][0] <= work[0] && need[j][1] <= work[1] && need[j][2] <= work[2]){
          sequence[index++] = j;
          finish[j] = 1;
          for(int k=0;k<m;k++){
            work[k] = work[k]+allocate[j][k];
          }
        }
      }
    }
  }
}

int main()
{
  
  for(int i=0;i<5;i++){
    for(int j=0;j<3;j++){
      need[i][j] = max[i][j] - allocate[i][j];
    }
    finish[i] = 0;
  }
  
  bankers();
  
  int flag = 0;
  for(int i=0;i<n;i++){
    if(finish[i]==0){
      flag=1;
      break;
    }
  }
  
  if(flag==0){
    printf("System is safe.\nSafe sequence: ");
    for(int i=0;i<n;i++){
      printf(" P%d ", sequence[i]);
    }
    
  }else{
    printf("System dont have safe sequence");
  }
  
  return 0;  
}

// 4. Deadlock Detection
#include <stdio.h>

int n=5, m=3;
	
	int alloc[5][3] = { { 0, 1, 0 }, 
						{ 2, 0, 0 }, 
						{ 3, 0, 3 }, 
						{ 2, 1, 1 }, 
						{ 0, 0, 2 } }; 

	int request[5][3] = { { 0, 0, 0 }, 
					 	  { 2, 0, 2 }, 
						  { 0, 0, 0 }, 
						  { 1, 0, 0 }, 
						  { 0, 0, 2 } }; 

int avail[3]={0,0,0};
int finish[5];

void detect(){
  for(int j=0;j<n;j++){
    for(int i=0;i<n;i++){
      if(finish[i]==0){
        if(request[i][0]<=avail[0] && request[i][1]<=avail[1] && request[i][2]<=avail[2]){
          finish[i] =1;
          for(int k=0;k<3;k++){
            avail[k] = avail[k]+alloc[i][k];
          }
        }
      }
    }
  }
  
  int flag =1;
  for(int i=0;i<n;i++){
    if(finish[i]==0){
      flag=0;
      printf("System have deadlock");
      break;
    }
  }
  
  if(flag==1)
      printf("System dont have deadlock");
}

int main()
{
  
  for(int i=0;i<n;i++){
    finish[i] =0;
  }
  
  detect();
  
}

// 5. Memory Allocation
// a. First Fit
#include <stdio.h>

int proNo, partNo;

void firstFit(int processes[], int partition[]){
  int allocated[proNo], unallo[proNo];
  int ind1=0, ind2=0;
  
  for(int i=0;i<proNo;i++){
    int flag =0;
    
    for(int j=0;j<partNo;j++){
      
      if(processes[i] <= partition[j]){
        partition[j] = partition[j] - processes[i];
        allocated[ind1++] = processes[i];
        flag = 1;
        break;
      }
      
    }
    if(flag==0){
      unallo[ind2++] = processes[i];
    }
    
  }
  printf("\nAllocated: ");
  for(int i=0;i<ind1;i++)
    printf(" %d ",allocated[i]);
    
  printf("\nUnallocated: ");
  for(int i=0;i<ind2;i++)
    printf(" %d ",unallo[i]);
  
  printf("\nMemory Partition: ");
  for(int i=0;i<partNo;i++)
      printf(" %d ", partition[i]);
  
}

int main()
{
  
  scanf("%d", &proNo);
  
  int processes[proNo];
  for(int i=0;i<proNo;i++)
      scanf("%d", &processes[i]);
  
  scanf("%d", &partNo);
  
  int partition[partNo];
  for(int i=0;i<partNo;i++)
      scanf("%d", &partition[i]);
  
  
  firstFit(processes, partition);
  

  return 0;
}
// b. Next Fit
#include <stdio.h>

int proNo, partNo;

void nextFit(int processes[], int partition[]){
  int allocation[proNo];
  int unalloc[proNo];
  
  for(int i=0;i<proNo;i++){
    allocation[i] = -1;
  }
  
  //to keep track of last allocation
  int j=0;
  
  for(int i=0;i<proNo;i++){
    int count=0;
    while(count<partNo){
      if(partition[j] >= processes[i]){
        allocation[i] = processes[i];
        partition[j] -= processes[i];
        break;
      }
      
      j=(j+1)%partNo;
      count++;
    }
  }
  
  for(int i=0;i<proNo;i++){
    printf(" %d ", allocation[i]);
  }
}

int main()
{
  
  scanf("%d", &proNo);
  
  int processes[proNo];
  for(int i=0;i<proNo;i++)
      scanf("%d", &processes[i]);
  
  scanf("%d", &partNo);
  
  int partition[partNo];
  for(int i=0;i<partNo;i++)
      scanf("%d", &partition[i]);
  
  nextFit(processes, partition);

  return 0;
}

// c. Best Fit
#include <stdio.h>

int partNo, proNo;

void bestFit(int processes[], int partition[]) {
    int allo[proNo], unallo[proNo];
    int ind1 = 0, ind2 = 0;
    
    for(int i = 0; i < proNo; i++) { 
        int minDiff = 1000;
        int minInd = -1;
        
        for(int j = 0; j < partNo; j++) {
          
            if(partition[j] >= processes[i]) {
                
                int diff = partition[j] - processes[i];
                
                if(diff < minDiff) {
                    minDiff = diff;
                    minInd = j;
                }
                
            }
        }
        
        if(minInd != -1) {
            partition[minInd] -= processes[i];
            allo[ind1++] = i;
        } else {
            unallo[ind2++] = i;
        }
    }
    
    printf("Allocated process indices: ");
    for(int i = 0; i < ind1; i++) {
        printf("P%d ", allo[i]);
    }
    printf("\nUnallocated process indices: ");
    for(int i = 0; i < ind2; i++) {
        printf("%d ", unallo[i]);
    }
    printf("\n");
}

int main() {
    scanf("%d", &proNo);
    int processes[proNo];
    for(int i = 0; i < proNo; i++)
        scanf("%d", &processes[i]);
    
    scanf("%d", &partNo);
    int partition[partNo];
    for(int i = 0; i < partNo; i++)
        scanf("%d", &partition[i]);
    
    bestFit(processes, partition);
    
    return 0;
}

// d. Worst Fit
#include <stdio.h>

int proNo, partNo;

void worstFit(int processes[], int partition[]){
  int ind1=0, ind2=0;
  int allo[proNo], unallo[proNo];
  
  for(int i=0;i<proNo;i++){
    int max =0;
    //find maximum partition
    for(int j=0;j<partNo;j++){
      if(partition[j] > partition[max] ){
          max = j;  
      }
    }
    
    if(partition[max]>=processes[i]){
      allo[ind1++] = processes[i];
      partition[max] -=processes[i];
    }else{
      unallo[ind2++] = processes[i];
    }
    
  }
  
  printf("\nUnallocated: ");
  for(int i=0;i<ind1;i++){
    printf(" %d ", allo[i]);
  }
  printf("\nUnallocated: ");
  for(int i=0;i<ind2;i++){
    printf(" %d ", unallo[i]);
  }
  
}

int main()
{
  
  scanf("%d", &proNo);
  
  int processes[proNo];
  for(int i=0;i<proNo;i++)
      scanf("%d", &processes[i]);
  
  scanf("%d", &partNo);
  
  int partition[partNo];
  for(int i=0;i<partNo;i++)
      scanf("%d", &partition[i]);
  
  worstFit(processes, partition);

  return 0;
}

// 6.Segmentation
// 7.Paging

// 8.Page Replacement
// a. FIFO
#include <stdio.h>

void FIFO(int pages[], int size,int frameNo){
  int frame[frameNo];
  int counter=0, pageHit = 0, pageMiss =0;
  
  for(int i=0;i<frameNo;i++){
    frame[i] =-1;
  }
  
  for(int i=0;i<size;i++){
       int flag = 0;
  //check page is already present in frame  
    for(int j=0;j<frameNo;j++){
   
     
      if(pages[i]==frame[j]){
        pageHit++;
        flag = 1;
        break;
      }
    }
    
    if(flag == 0){
      frame[counter] = pages[i];
      pageMiss++;
      counter = (counter+1)%3;
    }
  }
  
  printf(" PageMiss: %d \nPageHit: %d", pageMiss, pageHit);
  
}

int main()
{
  int n;
  scanf("%d", &n);
  int pages[n];
  for(int i=0;i<n;i++)
      scanf("%d", &pages[i]);
  
  int pageFrame=3;
  
  FIFO(pages, n, pageFrame);
  
    return 0;
}
// b.LRU
#include <stdio.h>

int getNotRecent(int pages[], int frames[], int index, int frameNo){
  int tempFrame[frameNo];
  int count =frameNo;
  int lru;
  
  for(int i=0;i<frameNo;i++){
    tempFrame[i] = frames[i];
  }
  
  for(int i=index;i>=0;i--){
    for(int j=0;j<frameNo;j++){
      if(pages[i] == tempFrame[i] && count>1){
        tempFrame[i] = -1;
        count--;
        // break;
      }
    }
  }
  
  for(int i=0;i<frameNo;i++){
    if(tempFrame[i]>=1){
      lru = i;
      break;
    }
  }
  
  return index;
}

void LRU(int pages[], int size, int frameNo){
  int pageMiss=0, pageHit=0, counter=0;
  int frames[frameNo];
  
  for(int i=0;i<frameNo;i++){
    frames[i] = 999;
  }
  
  for(int i=0;i<size;i++){ 
    int flag = 0;
    //if page is already in frame
    for(int j=0;j<frameNo; j++){
      if(pages[i]==frames[j]){
        pageHit++;
        flag = 1;
        break;
      }
    }
    //check if page frame is not totally filled.
    if(flag == 0){
      for(int j=0;j<frameNo;j++){
        if(frames[j]==999){
          frames[j] = pages[i];
          pageMiss++;
          flag=1;
          break;
        }
      }
    }
    
    if(flag==0){
      int notRecent = getNotRecent(pages, frames, i-1, frameNo);
      frames[notRecent] = pages[i];
      pageMiss++;
    }
  }
  
  printf("Page Miss: %d \nPagehit: %d", pageMiss, pageHit);
  
}

int main()
{
  int n;
  scanf("%d", &n);
  int pages[n];
  for(int i=0;i<n;i++)
      scanf("%d", &pages[i]);
  
  int pageFrame=3;
  
  LRU(pages, n, pageFrame);
  
    return 0;
}
// c.Optimal
#include <stdio.h>

int getNonFrequent(int pages[], int index, int pageSize, int frames[], int frameNo){
  int tempFrame[frameNo], ind;
  int count = frameNo;
  
  for(int i=0;i<frameNo;i++){
    tempFrame[i] = frames[i];
  }
  
  //keep upcoming non frequent 
  for(int i=index;i<pageSize;i++){
    for(int j=0;j<frameNo;j++){
      if(tempFrame[j] == pages[i] && count > 1){
        tempFrame[j] = -1;
        count--;
      }
    }
  }
  
  //find first element that has value more than -1 or 0;
  for(int i=0;i<frameNo;i++){
    if(tempFrame[i] > -1){
      ind = i;
      break;
    }
  }
  
  return ind;
}

void optimal(int pages[], int size, int frameNo){
  int frames[frameNo];
  int pageHit=0, pageMiss=0;
  
  for(int i=0;i<frameNo;i++){
    frames[i]=999;
  }
  
  for(int i=0;i<size;i++){
    int flag = 0;
    
    for(int j=0;j<frameNo;j++){
      if(pages[i]==frames[j]){
        pageHit++;
        flag=1;
        break;
      }  
    }
    
    //check if full or not
    if(flag==0){
      for(int j=0;j<frameNo;j++){
        if(frames[j]==999){
          frames[j] = pages[i];
          flag = 1;
          pageMiss++;
          break;
        }
      }
    }
    
    if(flag==0){
      int index = getNonFrequent(pages, i, size, frames, frameNo);
      frames[index] = pages[i];
      pageMiss++;
    }
  }
  
  printf("PageMiss: %d \nPageHit: %d", pageMiss, pageHit);
  
}

int main()
{
  int n;
  scanf("%d", &n);
  int pages[n];
  for(int i=0;i<n;i++)
      scanf("%d", &pages[i]);
      
  int frameNo;
  scanf("%d", &frameNo);
  
  optimal(pages, n, frameNo);
  
  return 0;
}

// 9.Disk Scheduling
// a.FCFS
#include<stdio.h>
#include<stdlib.h>

void FCFS(int arr[], int head, int size){
    int curr_track;
    int distance, seek_time=0;

    for(int i=0;i<size; i++){
        curr_track = arr[i];
        distance = abs(curr_track - head);
        seek_time +=distance;
        head = curr_track;
    }

    printf("Total Seek Time is: %d", seek_time);
}

int main(){
    int n, head;
    printf("No. of tracks: ");
    scanf("%d", &n);
    int arr[n];

    for(int i=0;i<n;i++){
        scanf("%d", &arr[i]);
    }

    printf("Head position: ");
    scanf("%d", &head);

    FCFS(arr, head, n);

    printf("\nSequence: ");
    for(int i=0;i<n;i++)
        printf(" %d ", arr[i]);

    return 0;
}

// b.SCAN
#include<stdio.h>
#include<stdlib.h>

void sort(int arr[], int size){
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(arr[i]<arr[j]){
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

int findNearest(int arr[], int head, int size){
    int index;
    for(int i=0;i<size;i++){
        if(head<arr[i]){
            index = i;
            break;
        }
    }

}

void scan(int arr[], int head, int size){
    int seek_count=0;
    int distance;
    int current, nearest;
    int sequence[size];
    int count =0;

    sort(arr, size);

    int index = findNearest(arr, head, size);
    nearest = arr[index];
    printf("Nearest: %d \n",nearest);

// for right side.
    for(int i=index;i<size;i++){
        current = arr[i];
        sequence[count++]=current;
        distance = abs(current-head);
        seek_count +=distance;
        head = current;
    }

//for remaining left side
    if(index>0){
        for(int i=index-1;i>=0;i--){
            current = arr[i];
            sequence[count++]=current;
            distance = abs(current-head);
            seek_count += distance;
            head = current;
        }
    }

    printf(" %d ", seek_count);
    printf("\nSequence: ");
    for(int i=0;i<size;i++){
        printf(" %d ", sequence[i]);
    }
}

int main(){
    int n;
    scanf("%d", &n);
    int arr[n];
    for(int i=0;i<n;i++){
        scanf("%d", &arr[i]);
    }

    int head;
    scanf("%d", &head);

    scan(arr, head, n);

    return 0;
}
// c.CSCAN
#include<stdio.h>
#include<stdlib.h>

void sort(int arr[], int n){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(arr[i]<arr[j]){
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

// what if head is greater than all numbers present in array
int findNearest(int arr[], int head, int size){
    int index;
    for(int i=0;i<size;i++){
        if(head<arr[i]){
            index = i;
            break;
        }
    }
    return index;
}

void cscan(int arr[], int head, int size){
    int current, seek_time =0;
    int sequence[size+1];
    int count=0, distance;

    sort(arr, size);

    int index = findNearest(arr, head, size);

// goto right track
    for(int i=index;i<size;i++){
        current = arr[i];
        sequence[count++]=current;
        distance = abs(current - head);
        seek_time +=distance;
        head = current;
    }

    //go back to zero 
    current = 0;
    sequence[count++] = current;
    distance = abs(current-head);
    seek_time += distance;
    head = current;
    
    // start again from zero
    for(int i=0;i<index;i++){
        current = arr[i];
        sequence[count++] = current;
        distance = abs(current-head);
        seek_time +=distance;
        head = current;
    }

    printf(" Seek Time: %d \nSequence: ",seek_time);
    for(int i=0;i<=size;i++){
        printf(" %d ",sequence[i]);
    }

}

int main(){
    int n;
    scanf("%d", &n);
    int arr[n];
    for(int i=0;i<n;i++){
        scanf("%d", &arr[i]);
    }

    int head;
    scanf("%d", &head);

    cscan(arr, head, n);

    return 0;
}
// d.SSTF
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void difference(int request[], int head, int diff[][2], int n) {
    for (int i = 0; i < n; i++) {
        diff[i][0] = abs(head - request[i]);
    }
}

int minimum(int diff[][2], int n) {
    int index = -1;
    int min = INT_MAX;

    for (int i = 0; i < n; i++) {
        if (!diff[i][1] && min > diff[i][0]) {
            min = diff[i][0];
            index = i;
        }
    }
    return index;
}

void sstf(int request[], int head, int n) {
    if (n == 0) {
        return;
    }
    
    int diff[n][2];
    int seekcount = 0;
    int sequence[n + 1];
    
    for (int i = 0; i < n; i++) {
        sequence[i] = head;
        difference(request, head, diff, n);
        int index = minimum(diff, n);
        diff[index][1] = 1;
        
        seekcount += diff[index][0]; 
        head = request[index];
    }
    sequence[n] = head;
    
    printf("Total number of seek operations = %d\n", seekcount);
    printf("Seek sequence is:\n");
    
    for (int i = 0; i <= n; i++) {
        printf("%d\n", sequence[i]);
    }
}

int main() {
    int n = 8;
    int proc[] = { 176, 79, 34, 60, 92, 11, 41, 114 };
    
    int head = 50;
    sstf(proc, head, n);
    
    return 0;
}
