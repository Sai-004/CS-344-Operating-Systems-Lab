The requirements are :-

1.while a writer is performing write operation no other can read (or) write the code.
2.while a read operation is being performed others can read the code.

f. First run repository.c , followed by writer.c , and then another instance of writer.c in another terminal. You'll see that the second writer is waiting for the first writer and writes only after completion of 1st writer.

g. First run repository.c , followed by writer.c , and then run reader.c in another terminal. You'll see that the reader is waiting for the writer and reads only after completion of the writer.

h. First run repository.c , followed by reader.c , and then another instance of reader.c in another terminal. You'll see that both the readers read the data simultaneously from the shared memory.
