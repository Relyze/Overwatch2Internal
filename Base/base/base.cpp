#include "../includes.h"

namespace base {
	void initializeBase() {
		if (!decrypt::find_gadget(decrypt::EIDOLON_GADGET)) {
			printf("Failed to find gadget\n");
			system("pause");
		}

		_beginthread((_beginthread_proc_type)update::Update, 0, 0);
		_beginthread((_beginthread_proc_type)features::doAim, 0, 0);
		_beginthread((_beginthread_proc_type)features::doMisc, 0, 0);

	}
}
