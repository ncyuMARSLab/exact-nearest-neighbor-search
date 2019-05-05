This project finds the ground truth of nearest neighbor for SIFT.  
It requires at least 128G RAM.  

It calculates 5 queries per time.  

In line 173  

    for (int q = 6415; q < QUERY_NUM; q += QUERY_PER_TIME) {
    
This project start at q-th query.  
