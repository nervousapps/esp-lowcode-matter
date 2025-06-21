// C++ Program to implement a queue using array
#include <stdio.h>

// defining the max size of the queue
#define MAX_SIZE 0x40

// Implement the queue data structure
class Queue {
public:
    int front;
    int rear;
    uint8_t arr[MAX_SIZE];

    // initializing pointers in the constructor
    Queue(): front(-1), rear(-1) {}

    // Function to check if the queue is empty or not
    bool isEmpty() { return front == -1 || front > rear; }

    // Function to check if the queue is full or not
    bool isFull() { return rear == MAX_SIZE - 1; }

    // Function to get the front element of the queue
    int getFront()
    {
        if (isEmpty()) {
            printf("\nQueue is empty\n");
            return -1;
        }
        return arr[front];
    }

    // Function to get the rear element of the queue
    int getRear()
    {
        if (isEmpty()) {
            printf("\nQueue is empty\n");
            return -1;
        }
        return arr[rear];
    }

    // Function to enqueue elements from the queue
    void enqueue(int val)
    {
        // Check overflow condition
        if (isFull()) {
            printf("\nQueue is full\n");
            return;
        }
        // if queue is empty, set front to 0
        if (isEmpty())
            front = 0;

        rear++;
        arr[rear] = val;
    }

    // Function to dequeue elements from the queue
    int dequeue()
    {
        // Check underflow condition
        if (isEmpty()) {
            printf("\nQueue is empty\n");
            return -1;
        }
        int ans = arr[front];
        front++;

        // if queue becomes empty, reset both pointers
        if (isEmpty())
            front = rear = -1;

        return ans;
    }

    // Display function to print the queue
    void display()
    {
        if (isEmpty()) {
            printf("\nQueue is empty\n");
            return;
        }
        printf("\nQueue:  ");
        for (int i = front; i <= rear; i++) {
            printf("%02X", arr[i]);
        }
    }
};