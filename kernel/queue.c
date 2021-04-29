// Luke Weber, 11398889
// CptS 460, LAB1
// Created 09/07/2016

// Enter p into queue by priority
int enqueue(PROC **queue, PROC *p)
{
  // Track insertion index
  int insert_index = 0;

  // Check root case
  if (!*queue) *queue = p;

  p->next; // Has no issue for some reason
  *queue->next = &p; // TODO perhaps because no allocation of queue?

  // Check existence priority of current PROC
  while (*queue->next && *queue->next->priority >= p->priority)
  {
    // Increment current PROC reference
    *queue++; insert_index++;
  } 

  // At this point, we want to insert after current PROC
  p->next = *queue->next;
  *queue->next = p->next;

  return insert_index;
}

// Remove and return first PROC pointer from queue
PROC *dequeue(PROC **queue)
{
  PROC *head = *queue;
  queue = &(*queue++);
  
  return head;
}
