#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <math.h>

struct uzenet {
    long cim;
    int x,y,z;
};

int main (int argc, char ** argv) {
    int kerek_szamot;
    if (argc > 2) {
	fprintf (stderr, "Hiba! Túl sok paraméter. \n");
	return -1;
    } else if (argc == 1) {
	kerek_szamot = 1;
    } else if (strcmp (argv[1], "-h") == 0) {
	fprintf (stderr, "Help: Kapcsoló nélkül bekérek 3 számot, -x kapcsoló esetén kihagyom a bekérést. \n");
	return 0;
    } else if (strcmp (argv[1], "-x") == 0) {
	kerek_szamot = 0;
    }

    struct uzenet uz1;
    uz1.cim = 0;
    int msgId = msgget (IPC_PRIVATE, 0777 | IPC_CREAT);
    int i, x = 9, y = 15, z = 30;
    
    for (i = 0; i < 4 && fork()>0; i++);
	if (i == 4) {
	    if (uz1.cim == 0) {
		if (kerek_szamot == 1) {
		    do {
			fprintf (stderr, "3 szam: \n");
			scanf ("%d\n%d\n%d", &x, &y, &z);
		    } while (x < 1 || x > 34 || y < 1 || y > 34 || z < 1 || z > 34);
		}
		uz1.x = x;
		uz1.y = y;
		uz1.z = z;
		uz1.cim = 1;
		msgsnd (msgId, &uz1, 3 * sizeof (int), 0777);
	    }
	    msgrcv (msgId, &uz1, 3 * sizeof (int), 5, 0777);
	    if (x == uz1.x && y == uz1.y && z == uz1.z) printf ("OK\n");
	    else printf ("Nem OK\n");
	    msgctl (msgId, IPC_RMID, NULL);
	}
	else {
	    if (i == 0) {
		msgrcv (msgId, &uz1, 3 * sizeof (int), 1, 0777);
		uz1.x = pow (uz1.x,5);
		uz1.y = pow (uz1.y,5);
		uz1.z = pow (uz1.z,5);
		uz1.cim = 2;
		msgsnd (msgId, &uz1, 3 * sizeof (int), 0777);
	    }
	    if (i == 1) {
		msgrcv (msgId, &uz1, 3 * sizeof (int), 2, 0777);
		uz1.x = uz1.x % 35;
		uz1.y = uz1.y % 35;
		uz1.z = uz1.z % 35;
		uz1.cim = 3;
		printf ("E%d\nE%d\nE%d\n", uz1.x, uz1.y, uz1.z);
		msgsnd (msgId, &uz1, 3 * sizeof (int), 0777);
	    }
	    if (i == 2) {
		msgrcv (msgId, &uz1, 3 * sizeof (int), 3, 0777);
		uz1.x = pow (uz1.x,5);
		uz1.y = pow (uz1.y,5);
		uz1.z = pow (uz1.z,5);
		uz1.cim = 4;
		msgsnd (msgId, &uz1, 3 * sizeof (int), 0777);
	    }
	    if (i == 3) {
		msgrcv (msgId, &uz1, 3 * sizeof (int), 4, 0777);
		uz1.x = uz1.x % 35;
		uz1.y = uz1.y % 35;
		uz1.z = uz1.z % 35;
		uz1.cim = 5;
		printf ("D%d\nD%d\nD%d\n", uz1.x, uz1.y, uz1.z);
		msgsnd (msgId, &uz1, 3 * sizeof (int), 0777);
	    }
	}
    return 0;
}
