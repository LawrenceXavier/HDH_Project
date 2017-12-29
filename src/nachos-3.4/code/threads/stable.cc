#include "stable.h"

STable::STable() {
    for (int i = 0; i < MAX_LOCK; ++i)
        lockTab[i] = NULL;
    bm = new BitMap(MAX_LOCK);
}

STable::~STable() {
    for (int i = 0; i < MAX_LOCK; ++i) {
		if (lockTab[i])
			delete lockTab[i];	
    }
    delete bm;
}

int STable::create(char *name, int init) {
    if (name == NULL || init < 0) return -1;
    int id;
    for (id = 0; id < MAX_LOCK; ++id) {
		if (lockTab[id]) {
        	if (strncmp(lockTab[id]->getName(), name, MAX_SEMNAME_LEN) == 0) 
				return -1;
		}
    }
    
	id = bm->Find();
    if (id == -1) return -1;
	
    lockTab[id] = new Sem(name, init);
	if (lockTab[id]->isExist)
		return 0;
	
	delete lockTab[id];
	bm->Clear(id);
	return -1;
}

int STable::wait(char *name) {
    if (name == NULL) return -1;
    for (int i = 0; i < MAX_LOCK; ++i) {
		if (lockTab[i]) {
			if (strncmp(lockTab[i]->getName(), name, MAX_SEMNAME_LEN) == 0) {
				lockTab[i]->wait();
				return 0;
			}
		}
    }
    return -1;
}

int STable::signal(char *name) {
    if (name == NULL) return -1;
    for (int i = 0; i < MAX_LOCK; ++i) {
        if (lockTab[i]) {
			if (strncmp(lockTab[i]->getName(), name, MAX_SEMNAME_LEN) == 0) {
				lockTab[i]->signal();
				return 0;
			}
		}
	}
    return -1;
}
