#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <chrono>
#include <assert.h>
#include <sys/time.h>
#include <iomanip>    



using namespace std;

// global variables
pthread_mutex_t northStraightMutex;
pthread_mutex_t northLeftMutex;
pthread_mutex_t northRightMutex;
pthread_mutex_t eastStraightMutex;
pthread_mutex_t eastLeftMutex;
pthread_mutex_t eastRightMutex;
pthread_mutex_t westStraightMutex;
pthread_mutex_t westLeftMutex;
pthread_mutex_t westRightMutex;
pthread_mutex_t southStraightMutex;
pthread_mutex_t southLeftMutex;
pthread_mutex_t southRightMutex;

pthread_cond_t northStraightCond;
pthread_cond_t northLeftCond;
pthread_cond_t northRightCond;
pthread_cond_t eastStraightCond;
pthread_cond_t eastLeftCond;
pthread_cond_t eastRightCond;
pthread_cond_t westStraightCond;
pthread_cond_t westLeftCond;
pthread_cond_t westRightCond;
pthread_cond_t southStraightCond;
pthread_cond_t southLeftCond;
pthread_cond_t southRightCond;
// clock start timer
auto start = chrono::steady_clock::now();

// Given by project rubric, with some modifications
typedef struct _directions {
    int carID;
	char dir_original;
	char dir_target;
	float arrivalTime;
	int turn;
} directions;

// condition 1 = arriving, 2 = crossing, 3 = exiting
// Prints time, car id, dir_original, dir_target all formatted.
void ElapsedTime(directions dir, int condition){
    auto end = chrono::steady_clock::now();
	cout << fixed;
	cout << setprecision(2);
    cout << "Time " << chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0<< ": Car "
		<< dir.carID << " (->" << dir.dir_original << " ->" << dir.dir_target << ") ";
	if(condition == 1){
		cout << "arriving\n";
	}
	else if(condition == 2){
		cout << "		crossing\n";
	}
	else if(condition == 3){
		cout << "				exiting\n";
	}
	else{
		cout << "ERROR\n";
	}
}

// returns time, used for arriving cars
float GrabTime(){
    auto end = chrono::steady_clock::now();
	float getTime = chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0;
	return getTime;
}

// mutex's to lock/unlock/signal if car's target direction is straight
bool StraightTurn(directions dir, bool unlock) {
	//locking
	if (dir.dir_original == dir.dir_target && !unlock) {
		if (dir.dir_original == 'N') {
			pthread_mutex_lock(&eastStraightMutex);
			pthread_mutex_lock(&southLeftMutex);
			pthread_mutex_lock(&westStraightMutex);
			pthread_mutex_lock(&westLeftMutex);
			pthread_mutex_lock(&westRightMutex);
			
			pthread_cond_wait(&eastStraightCond, &eastStraightMutex);
			pthread_cond_wait(&southLeftCond, &southLeftMutex);
			pthread_cond_wait(&westStraightCond, &westStraightMutex);
			pthread_cond_wait(&westLeftCond, &westLeftMutex);
			pthread_cond_wait(&westRightCond, &westRightMutex);
			return true;
		}
		if (dir.dir_original == 'E') {
			pthread_mutex_lock(&northStraightMutex);
			pthread_mutex_lock(&westLeftMutex);
			pthread_mutex_lock(&southLeftMutex);
			pthread_mutex_lock(&southStraightMutex);
			pthread_mutex_lock(&southRightMutex);

			pthread_cond_wait(&northStraightCond, &northStraightMutex);
			pthread_cond_wait(&westLeftCond, &westLeftMutex);
			pthread_cond_wait(&southLeftCond, &southLeftMutex);
			pthread_cond_wait(&southStraightCond, &southStraightMutex);
			pthread_cond_wait(&southRightCond, &southRightMutex);
			return true;
		}
		if (dir.dir_original == 'S') {
			pthread_mutex_lock(&westStraightMutex);
			pthread_mutex_lock(&northLeftMutex);
			pthread_mutex_lock(&eastStraightMutex);
			pthread_mutex_lock(&eastLeftMutex);
			pthread_mutex_lock(&eastRightMutex);

			pthread_cond_wait(&westStraightCond, &westStraightMutex);
			pthread_cond_wait(&northLeftCond, &northLeftMutex);
			pthread_cond_wait(&eastStraightCond, &eastStraightMutex);
			pthread_cond_wait(&eastLeftCond, &eastLeftMutex);
			pthread_cond_wait(&eastRightCond, &eastRightMutex);
			return true;
		}
		if (dir.dir_original == 'W') {
			pthread_mutex_lock(&southStraightMutex);
			pthread_mutex_lock(&eastLeftMutex);
			pthread_mutex_lock(&northLeftMutex);
			pthread_mutex_lock(&northStraightMutex);
			pthread_mutex_lock(&northRightMutex);

			pthread_cond_wait(&southStraightCond, &southStraightMutex);
			pthread_cond_wait(&eastLeftCond, &eastLeftMutex);
			pthread_cond_wait(&northLeftCond, &northLeftMutex);
			pthread_cond_wait(&northStraightCond, &northStraightMutex);
			pthread_cond_wait(&northRightCond, &northRightMutex);
			return true;
		}
	}
	// unlocking
	if (dir.dir_original == dir.dir_target && unlock) {
		if (dir.dir_original == 'N') {
			pthread_cond_signal(&eastStraightCond);
			pthread_cond_signal(&southLeftCond);
			pthread_cond_signal(&westStraightCond);
			pthread_cond_signal(&westLeftCond);
			pthread_cond_signal(&westRightCond);

			pthread_mutex_unlock(&eastStraightMutex);
			pthread_mutex_unlock(&southLeftMutex);
			pthread_mutex_unlock(&westStraightMutex);
			pthread_mutex_unlock(&westLeftMutex);
			pthread_mutex_unlock(&westRightMutex);
		}
		if (dir.dir_original == 'E') {
			pthread_cond_signal(&northStraightCond);
			pthread_cond_signal(&westLeftCond);
			pthread_cond_signal(&southLeftCond);
			pthread_cond_signal(&southStraightCond);
			pthread_cond_signal(&southRightCond);

			pthread_mutex_unlock(&northStraightMutex);
			pthread_mutex_unlock(&westLeftMutex);
			pthread_mutex_unlock(&southLeftMutex);
			pthread_mutex_unlock(&southStraightMutex);
			pthread_mutex_unlock(&southRightMutex);
		}
		if (dir.dir_original == 'S') {
			pthread_cond_signal(&westStraightCond);
			pthread_cond_signal(&northLeftCond);
			pthread_cond_signal(&eastStraightCond);
			pthread_cond_signal(&eastLeftCond);
			pthread_cond_signal(&eastRightCond);

			pthread_mutex_unlock(&westStraightMutex);
			pthread_mutex_unlock(&northLeftMutex);
			pthread_mutex_unlock(&eastStraightMutex);
			pthread_mutex_unlock(&eastLeftMutex);
			pthread_mutex_unlock(&eastRightMutex);
		}
		if (dir.dir_original == 'W') {
			pthread_cond_signal(&southStraightCond);
			pthread_cond_signal(&eastLeftCond);
			pthread_cond_signal(&northLeftCond);
			pthread_cond_signal(&northStraightCond);
			pthread_cond_signal(&northRightCond);

			pthread_mutex_unlock(&southStraightMutex);
			pthread_mutex_unlock(&eastLeftMutex);
			pthread_mutex_unlock(&northLeftMutex);
			pthread_mutex_unlock(&northStraightMutex);
			pthread_mutex_unlock(&northRightMutex);
		}
	}
	return false;
}
// mutex's to lock/unlock/signal if car's target direction is right
bool RightTurn(directions dir, bool unlock) {
	//locking
	if (dir.dir_original == 'N' && dir.dir_target == 'E' && !unlock) {
		pthread_mutex_lock(&southLeftMutex);
		pthread_cond_wait(&southLeftCond, &southLeftMutex);
		return true;
	}
	if (dir.dir_original == 'E' && dir.dir_target == 'S' && !unlock) {
		pthread_mutex_lock(&westLeftMutex);
		pthread_cond_wait(&westLeftCond, &westLeftMutex);
		return true;
	}
	if (dir.dir_original == 'W' && dir.dir_target == 'N' && !unlock) {
		pthread_mutex_lock(&eastLeftMutex);
		pthread_cond_wait(&eastLeftCond, &eastLeftMutex);
		return true;
	}
	if (dir.dir_original == 'S' && dir.dir_target == 'W' && !unlock) {
		pthread_mutex_lock(&northLeftMutex);
		pthread_cond_wait(&northLeftCond, &northLeftMutex);
		return true;
	}
	//unlocking
	if (dir.dir_original == 'N' && dir.dir_target == 'E' && unlock) {
		pthread_cond_signal(&southLeftCond);
		pthread_mutex_unlock(&southLeftMutex);
	}
	if (dir.dir_original == 'E' && dir.dir_target == 'S' && unlock) {
		pthread_cond_signal(&westLeftCond);
		pthread_mutex_unlock(&westLeftMutex);
	}
	if (dir.dir_original == 'W' && dir.dir_target == 'N' && unlock) {
		pthread_cond_signal(&eastLeftCond);
		pthread_mutex_unlock(&eastLeftMutex);
	}
	if (dir.dir_original == 'S' && dir.dir_target == 'W' && unlock) {
		pthread_cond_signal(&northLeftCond);
		pthread_mutex_unlock(&northLeftMutex);
	}
	return false;
}
// mutex's to lock/unlock/signal if car's target direction is left
bool LeftTurn(directions dir, bool unlock) {
	//locking
	if (dir.dir_original == 'N' && dir.dir_target == 'W' && !unlock) {
		pthread_mutex_lock(&eastStraightMutex);
		pthread_mutex_lock(&westLeftMutex);
		pthread_mutex_lock(&eastLeftMutex);
		pthread_mutex_lock(&southStraightMutex);
		pthread_mutex_lock(&southRightMutex);

		pthread_cond_wait(&eastStraightCond, &eastStraightMutex);
		pthread_cond_wait(&westLeftCond, &westLeftMutex);
		pthread_cond_wait(&eastLeftCond, &eastLeftMutex);
		pthread_cond_wait(&southStraightCond, &southStraightMutex);
		pthread_cond_wait(&southRightCond, &southRightMutex);
		return true;
	}
	if (dir.dir_original == 'S' && dir.dir_target == 'E' && !unlock) {
		pthread_mutex_lock(&westStraightMutex);
		pthread_mutex_lock(&eastLeftMutex);
		pthread_mutex_lock(&westLeftMutex);
		pthread_mutex_lock(&northStraightMutex);
		pthread_mutex_lock(&northRightMutex);

		pthread_cond_wait(&westStraightCond, &westStraightMutex);
		pthread_cond_wait(&eastLeftCond, &eastLeftMutex);
		pthread_cond_wait(&westLeftCond, &westLeftMutex);
		pthread_cond_wait(&northStraightCond, &northStraightMutex);
		pthread_cond_wait(&northRightCond, &northRightMutex);
		return true;
	}
	if (dir.dir_original == 'E' && dir.dir_target == 'S' && !unlock) {
		pthread_mutex_lock(&southStraightMutex);
		pthread_mutex_lock(&northLeftMutex);
		pthread_mutex_lock(&southLeftMutex);
		pthread_mutex_lock(&westStraightMutex);
		pthread_mutex_lock(&eastRightMutex);

		pthread_cond_wait(&southStraightCond, &southStraightMutex);
		pthread_cond_wait(&northLeftCond, &northLeftMutex);
		pthread_cond_wait(&southLeftCond, &southLeftMutex);
		pthread_cond_wait(&westStraightCond, &westStraightMutex);
		pthread_cond_wait(&eastRightCond, &eastRightMutex);
		return true;
	}
	if (dir.dir_original == 'W' && dir.dir_target == 'N' && !unlock) {
		pthread_mutex_lock(&northStraightMutex);
		pthread_mutex_lock(&southLeftMutex);
		pthread_mutex_lock(&northLeftMutex);
		pthread_mutex_lock(&eastStraightMutex);
		pthread_mutex_lock(&westRightMutex);

		pthread_cond_wait(&northStraightCond, &northStraightMutex);
		pthread_cond_wait(&southLeftCond, &southLeftMutex);
		pthread_cond_wait(&northLeftCond, &northLeftMutex);
		pthread_cond_wait(&eastStraightCond, &eastStraightMutex);
		pthread_cond_wait(&westRightCond, &westRightMutex);
		return true;
	}
	//unlocking
	if (dir.dir_original == 'N' && dir.dir_target == 'W' && unlock) {
		pthread_cond_signal(&eastStraightCond);
		pthread_cond_signal(&westLeftCond);
		pthread_cond_signal(&eastLeftCond);
		pthread_cond_signal(&southStraightCond);
		pthread_cond_signal(&southRightCond);

		pthread_mutex_unlock(&eastStraightMutex);
		pthread_mutex_unlock(&westLeftMutex);
		pthread_mutex_unlock(&eastLeftMutex);
		pthread_mutex_unlock(&southStraightMutex);
		pthread_mutex_unlock(&southRightMutex);
	}
	if (dir.dir_original == 'S' && dir.dir_target == 'E' && unlock) {
		pthread_cond_signal(&westStraightCond);
		pthread_cond_signal(&eastLeftCond);
		pthread_cond_signal(&westLeftCond);
		pthread_cond_signal(&northStraightCond);
		pthread_cond_signal(&northRightCond);

		pthread_mutex_unlock(&westStraightMutex);
		pthread_mutex_unlock(&eastLeftMutex);
		pthread_mutex_unlock(&westLeftMutex);
		pthread_mutex_unlock(&northStraightMutex);
		pthread_mutex_unlock(&northRightMutex);
	}
	if (dir.dir_original == 'E' && dir.dir_target == 'S' && unlock) {
		pthread_cond_signal(&southStraightCond);
		pthread_cond_signal(&northLeftCond);
		pthread_cond_signal(&southLeftCond);
		pthread_cond_signal(&westStraightCond);
		pthread_cond_signal(&eastRightCond);

		pthread_mutex_unlock(&southStraightMutex);
		pthread_mutex_unlock(&northLeftMutex);
		pthread_mutex_unlock(&southLeftMutex);
		pthread_mutex_unlock(&westStraightMutex);
		pthread_mutex_unlock(&eastRightMutex);
	}
	if (dir.dir_original == 'W' && dir.dir_target == 'N' && unlock) {
		pthread_cond_signal(&northStraightCond);
		pthread_cond_signal(&southLeftCond);
		pthread_cond_signal(&northLeftCond);
		pthread_cond_signal(&eastStraightCond);
		pthread_cond_signal(&westRightCond);

		pthread_mutex_unlock(&northStraightMutex);
		pthread_mutex_unlock(&southLeftMutex);
		pthread_mutex_unlock(&northLeftMutex);
		pthread_mutex_unlock(&eastStraightMutex);
		pthread_mutex_unlock(&westRightMutex);
	}
	return false;
}

// head of line lock, so cars behind the current car can start 
// crossing if no other cars are lined up
void HeadOfLineLock(directions dir){
	if(dir.dir_original == 'N'){
		pthread_cond_signal(&northStraightCond);
		pthread_mutex_unlock(&northStraightMutex);
	}
	if(dir.dir_original == 'E'){
		pthread_cond_signal(&eastStraightCond);
		pthread_mutex_unlock(&eastStraightMutex);
	}
	if(dir.dir_original == 'S'){
		pthread_cond_signal(&southStraightCond);
		pthread_mutex_unlock(&southStraightMutex);
	}
	if(dir.dir_original == 'W'){
		pthread_cond_signal(&westStraightCond);
		pthread_mutex_unlock(&westStraightMutex);
	}
}

// Given in p3 rubric
double GetTime() {
    struct timeval t;
    int rc = gettimeofday(&t, NULL);
    assert(rc == 0);
    return (double)t.tv_sec + (double)t.tv_usec/1e6;
}

// Given in p3 rubric
void Spin(int howlong) {
    double t = GetTime();
    while ((GetTime() - t) < (double)howlong); // do nothing in loop
}

int ArriveIntersection(directions dir) {
	//cars going straight
	// left = 3, straight = 2, right = 1
	int turn = 500;

	// do nothing until arrived
	while(dir.arrivalTime>=GrabTime());

	// Finds out if car is going left, right, or straight, also locks mutex
	if(RightTurn(dir, false)){
		turn = 1;
	}
	if(StraightTurn(dir, false)){
		turn = 2;
	}
	if(LeftTurn(dir, false)){
		turn = 3;
	}
	// prints time
	ElapsedTime(dir,1);
	return turn;
}
void CrossIntersection(directions dir, int turn) {
	// left = 3, straight = 2, right = 1
	HeadOfLineLock(dir);
	ElapsedTime(dir,2);
	// spin cpu for certain amount of seconds depending on direction
	if(turn == 1){
		Spin(1);
	}
	else if (turn == 2){
		Spin(2);
	}
	else if(turn == 3){
		Spin(3);
	}
	
	
	
}
void ExitIntersection(directions dir) {
	// cleans up mutex and prints exit time, true = unlock/signal
	StraightTurn(dir, true);
	RightTurn(dir, true);
	LeftTurn(dir, true);
	ElapsedTime(dir, 3);

}
void* Car(void* dirVoid) {
	// converting void* back to _directions
	struct _directions dir = *(struct _directions*)dirVoid;
	int turn = 500;
	turn = ArriveIntersection(dir);
	CrossIntersection(dir,turn);
	ExitIntersection(dir);
	return 0;
}



int main() {
    directions myCar[8];
	// testing arrays given by the rubric
    char testDirOriginal [] = {'N', 'N', 'N', 'S', 'S', 'N', 'E', 'W'};
    char testDirTarget [] = {'N', 'N', 'W', 'S', 'E', 'N', 'N', 'N'};
	float testArrivalTime[] = {1.1, 2.0, 3.3, 3.5, 4.2, 4.4, 5.7, 5.9};
	// loading up the directions array so each thread gets information on what to do
    for(int i = 0; i<8; i++){
        myCar[i].carID=i;
        myCar[i].dir_original = testDirOriginal[i];
        myCar[i].dir_target = testDirTarget[i];
		myCar[i].arrivalTime = testArrivalTime[i];
    }
	// inits
	pthread_mutex_init(&northStraightMutex, NULL);
	pthread_mutex_init(&northLeftMutex, NULL);
	pthread_mutex_init(&northRightMutex, NULL);
	pthread_mutex_init(&eastStraightMutex, NULL);
	pthread_mutex_init(&eastLeftMutex, NULL);
	pthread_mutex_init(&eastRightMutex, NULL);
	pthread_mutex_init(&westStraightMutex, NULL);
	pthread_mutex_init(&westLeftMutex, NULL);
	pthread_mutex_init(&westRightMutex, NULL);
	pthread_mutex_init(&southStraightMutex, NULL);
	pthread_mutex_init(&southLeftMutex, NULL);
	pthread_mutex_init(&southRightMutex, NULL);

	pthread_cond_init(&northStraightCond, NULL);
	pthread_cond_init(&northLeftCond, NULL);
	pthread_cond_init(&northRightCond, NULL);
	pthread_cond_init(&eastStraightCond, NULL);
	pthread_cond_init(&eastLeftCond, NULL);
	pthread_cond_init(&eastRightCond, NULL);
	pthread_cond_init(&westStraightCond, NULL);
	pthread_cond_init(&westLeftCond, NULL);
	pthread_cond_init(&westRightCond, NULL);
	pthread_cond_init(&southStraightCond, NULL);
	pthread_cond_init(&southLeftCond, NULL);
	pthread_cond_init(&southRightCond, NULL);
	pthread_t *carThread;
    
    carThread = (pthread_t *)malloc(sizeof(*carThread));
	// creation of threads
	for(int i = 0; i < 8; i++){
		pthread_create(carThread, NULL, Car, (void *)&myCar[i]);
	}

	// thread cleanup
	pthread_mutex_destroy(&northStraightMutex);
	pthread_mutex_destroy(&northLeftMutex);
	pthread_mutex_destroy(&northRightMutex);
	pthread_mutex_destroy(&eastStraightMutex);
	pthread_mutex_destroy(&eastLeftMutex);
	pthread_mutex_destroy(&eastRightMutex);
	pthread_mutex_destroy(&westStraightMutex);
	pthread_mutex_destroy(&westLeftMutex);
	pthread_mutex_destroy(&westRightMutex);
	pthread_mutex_destroy(&southStraightMutex);
	pthread_mutex_destroy(&southLeftMutex);
	pthread_mutex_destroy(&southRightMutex);

	pthread_cond_destroy(&northStraightCond);
	pthread_cond_destroy(&northLeftCond);
	pthread_cond_destroy(&northRightCond);
	pthread_cond_destroy(&eastStraightCond);
	pthread_cond_destroy(&eastLeftCond);
	pthread_cond_destroy(&eastRightCond);
	pthread_cond_destroy(&westStraightCond);
	pthread_cond_destroy(&westLeftCond);
	pthread_cond_destroy(&westRightCond);
	pthread_cond_destroy(&southStraightCond);
	pthread_cond_destroy(&southLeftCond);
	pthread_cond_destroy(&southRightCond);

	
	pthread_exit(NULL);

	return 0;
}

