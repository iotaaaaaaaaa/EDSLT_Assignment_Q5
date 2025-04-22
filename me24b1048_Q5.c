// EDSLT Assignment-1
// Cargo Drone Traffic Controller - Question No.5
// Kovuru Girish Chandra  | ME24B1048 Batch - 5
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max 6   // max size for stack, queue, log etc

// global declarations for queue  - normal deliveries
char *delivery_requests[max];
int front = -1, rear = -1, count_delivery_requests = 0;

// global declarations for stack  - urgent deliveries
char *Priority_Dispatcher[max];
int top = -1, count_urgent_deliveries = 0;

// temp char pointer
char *temp;


// global declarations for log (fixed size array)
char *flight_log[max];
int log_size = 0;     // number of deliveries logged so far


// singly linked list for overloaded drones
struct sll {
	char drone_id[200];
	struct sll *next;
} *head = NULL, *temp1, *newnode;

// doubly linked list for serviced drones
struct dll {
	char drone_id[200];
	struct dll *next, *prev;
} *start = NULL, *last = NULL, *newdll;

// circular linked list for emergency rerouting
struct cll {
	char drone_id[200];
	struct cll *next;
} *headc = NULL, *temp2, *newc;

// check if queue full
int qisfull() {
	return count_delivery_requests == max;
}

// check if queue empty
int qisempty() {
	return front == -1;
}

// enqueue into normal delivery queue
void enqueue(char *item) {
	if (qisfull()) {
		printf("Queue full! Cannot add more deliveries.\n");
		return;
	}
	if (front == -1) front = 0;
	rear++;
	delivery_requests[rear] = item;
	count_delivery_requests++;
	printf("Added to queue: %s\n", item);
}

// dequeue from queue
char* dequeue() {
	if (qisempty()) {
		printf("Queue empty!\n");
		return NULL;
	}
	temp = delivery_requests[front];
	if (front == rear) front = rear = -1;
	else front++;
	count_delivery_requests--;
	return temp;
}

// push into urgent delivery stack
void push() {
	if (top == max - 1) {
		printf("Urgent delivery stack full!\n");
		return;
	}
	char *item = dequeue();
	if (item != NULL) {
		top++;
		Priority_Dispatcher[top] = item;
		count_urgent_deliveries++;
		printf("Moved to urgent stack: %s\n", item);
	}
}

// pop from urgent stack and dispatch
void pop() {
	if (top == -1) {
		printf("No urgent deliveries!\n");
		return;
	}
	char *dispatched = Priority_Dispatcher[top];
	printf("Urgent Delivery Dispatched: %s\n", dispatched);
	top--;
	count_urgent_deliveries--;
}

// --------------------------- Log Unit Functions ---------------------------

void log_delivery(char *item) {
	if (log_size < max) {
		flight_log[log_size++] = item;
	} else {
		// shift all left (archive oldest)
		for (int i = 0; i < max - 1; i++) {
			flight_log[i] = flight_log[i + 1];
		}
		flight_log[max - 1] = item;
	}
	printf("Logged delivery: %s\n", item);
}

void view_log() {
	printf("Flight Log:\n");
	for (int i = 0; i < log_size; i++) {
		printf("%d. %s\n", i + 1, flight_log[i]);
	}
}

void add_overloaded(char *id) {
	newnode = (struct sll*)malloc(sizeof(struct sll));
	strcpy(newnode->drone_id, id);
	newnode->next = NULL;
	if (head == NULL) head = temp1 =  newnode;
	else {
		temp1->next = newnode;
		temp1 = newnode;
	}
	printf("Added to overloaded list: %s\n", id);
}

void move2serviced(char *id) {
	struct sll *prev = NULL;
	temp1 = head;
	while (temp1 != NULL && strcmp(temp1->drone_id, id) != 0) {
		prev = temp1;
		temp1 = temp1->next;
	}
	if (temp1 == NULL) {
		printf("Drone not found in overloaded list.\n");
		return;
	}
	if (prev == NULL) head = temp1->next;
	else prev->next = temp1->next;

	newdll = (struct dll*)malloc(sizeof(struct dll));
	strcpy(newdll->drone_id, id);
	newdll->prev = NULL;
	newdll->next = NULL;
	if (start == NULL) start = last = newdll;
	else {
		last->next = newdll;
		newdll->prev = last;
		last = newdll;
	}
	printf("Moved to serviced list: %s\n", id);
}

void traverse_forward() {
	struct dll *temp = start;
	printf("Serviced Drones (Forward): ");
	while (temp != NULL) {
		printf("%s ", temp->drone_id);
		temp = temp->next;
	}
	printf("\n");
}

void traverse_backward() {
	struct dll *temp = last;
	printf("Serviced Drones (Backward): ");
	while (temp != NULL) {
		printf("%s ", temp->drone_id);
		temp = temp->prev;
	}
	printf("\n");
}

void add_emergency(char *id) {
	newc = (struct cll*)malloc(sizeof(struct cll));
	strcpy(newc->drone_id, id);
	newc->next = NULL;

	if (headc == NULL) {
		headc = newc;
		newc->next = headc;
	} else {
		temp2 = headc;
		while (temp2->next != headc)
			temp2 = temp2->next;
		temp2->next = newc;
		newc->next = headc;
	}
	printf("Emergency reroute: %s\n", id);
}

void traverse_emergency_twice() {
	if (headc == NULL) {
		printf("No emergency drones.\n");
		return;
	}
	temp2 = headc;
	printf("Emergency Drones Traversed Twice:\n");
	for (int i = 0; i < 2 * max; i++) {
		printf("%s -> ", temp2->drone_id);
		temp2 = temp2->next;
		if (temp2 == headc && i >= max - 1) break;
	}
	printf("...\n");
}

int main() {

	enqueue("Food");
	enqueue("Medicine");
	enqueue("Tools");       // execution of part a
	enqueue("Water");
	enqueue("Parts");
	enqueue("Fuel");

	printf("\n");

	for (int i = 0; i < 6; i++) {
		push();   // move all to urgent stack
	}

	printf("\n");

	for (int i = 0; i < 6; i++) {
		pop();    // dispatch from stack
	}
	printf("\n");

	//creativity bonus for a

	// Using LIFO for urgent deliveries ensures that the most recent emergency like Fuel for a half-empty drone is prioritized instantly.
	// This mimics real-world emergency handling, where latest issues are often most time-sensitive.



	log_delivery("Del1");
	log_delivery("Del2");
	log_delivery("Del3");
	log_delivery("Del4");
	log_delivery("Del5");
	log_delivery("Del6");
	log_delivery("Del7");
	log_delivery("Del8");
	printf("\n");
	view_log();     // displayin  the log at once
	printf("\n");

	// Creativity Bonus for b
	// Archiving older deliveries in the log allows the system to stay memory-efficient and compliant with aviation storage limits.
	// It reflects real practices in airspace control, where only the most recent logs are actively stored.

	add_overloaded("Drone3");
	add_overloaded("Drone6");
	move2serviced("Drone3");
	traverse_forward();
	traverse_backward();
	printf("\n");

	// Creativity Bonus for c
	// Drone3 was overloaded with tools causing flight instability. It was recalibrated by drone techs, lightened, and moved to serviced state.
	// This improves safety and ensures reliable operation under load constraints.

	add_emergency("Drone1");
	add_emergency("Drone4");
	traverse_emergency_twice();
	printf("\n");

	// Creativity Bonus for d
	// Drone4 was rerouted due to a sudden storm alert. Engineers equipped it with a wind-resistant shell, enabling safe navigation mid-storm.
	// Such dynamic reroutes improve drone resilience and package safety.

}
