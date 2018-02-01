int
waitpid(int pidInput, int* stat, int options) {
        struct proc *p;
        int pid;
        struct proc *curproc = myproc();

        acquire(&ptable.lock);

        for(p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
                if(p->pid == pidInput) {
                        pid = p->pid;   // Grab p's pid
                        break;          // Keep p on that process
                }
        }

        //      If we didn't find the pid or the current process somehow died, return -1
        if(p == &ptable.proc[NPROC] || curproc->killed) {
                release(&ptable.lock);
                return -1;
        }

        for (;;) {
                if (p->status == ZOMBIE) {
                        kfree(p->kstack);       // kfree defined in kalloc.c
                                                // frees the process's kernel stack
                        p->kstack = 0;          // process's kernel stack points to NULL
                        freevm(p->pgdir);       // frees a page table
                        p->pid = 0;             // Set child's pid to 0
                        p->parent = 0;
                        p->name[0] = 0;
                        p->killed = 0;          // if non-zero, has been killed. Resurrecting it
                        p->state = UNUSED;

                        //      Lab1: If not null, update status pointer value with child's exit status
                        if (stat != 0) {                //      NULL is not defined yet
                                *stat = p->status;
                        }

                        release(&ptable.lock);          // Release lock
                        return pid;
                }

                //      Wait for child to exit
                sleep(curproc, &ptable.lock);
        }
}
