/**
 * This structure of object contains identifier of object id, and its x and
 * y coordinate.
 */
struct obj_t {
    int id;
    float x;
    float y;
};

/**
 * This structure of cluster contains size of cluster (number of objects in
 * cluster), capacity of cluster (maximal number of objects in
 * cluster), and pointer to the array of objects.
 */
struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/** @defgroup Functions_work_with_objects
 *  @brief The functions used for work with objects.
 *  @{
 */


/**
 * This function initialises cluster 'c' by allocating memory for
 * the given capacity (cap) of a cluster. If array of objects has
 * pointer NULL, capacity of the cluster is 0.
 * @brief This function initialises cluster.
 * @pre c != NULL
 * @pre cap >= 0
 * @param[out] c   This parameter is a pointer to the cluster of
 *                 objects for which  memory will be allocated.
 * @param[in]  cap This parameter represents capacity of a given cluster.
 * @post Memory has been allocated.
 * @return nothing
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * This function frees the memory allocated for the objects of the
 * given cluster and initialises it to a empty cluster.
 * @brief This function clears cluster.
 * @param  c       This parameter is a pointer to the cluster
 *                 objects of which will be cleared.
 * @return nothing
 * @post Memory has been freed.
 */
void clear_cluster(struct cluster_t *c);

/** @} */

/** @defgroup Constants
 *  @{
 */

/**
 * This constant is used for easier work with clusters. Smaller
 * number of reallocations is required.
 * @brief Constant for reallocation.
 */
extern const int CLUSTER_CHUNK;

/** @} */

/** @defgroup Functions_work_with_clusters
 *  @brief The functions used for work with clusters.
 *  @{
 */

/**
 * This function resizes the given cluster c. It reallocates
 * array of objects of the given cluster. New capacity is set
 * due to value given by parameter.
 * @brief This function resizes cluster.
 * @pre c is initiated
 * @pre c->capacity >= 0
 * @pre new_cap >= 0
 * @param     c        This parameter is a pointer to the cluster
 *                     which will be resized.
 * @param[in] new_cap  This parameter represents capacity of the final cluster.
 * @post Cluster has been resized.
 * @return nothing
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * This function appends the given object obj at the end of the
 * cluster c. If needed, it also resizes the cluster c.
 * @brief This function appends object to cluster.
 * @param     c       This parameter is a pointer to the cluster
 *                    which will be resiyed.
 * @param[in] obj     This parameter represents object which will be
 *                    added to the given cluster.
 * @post The object obj has been at the end of the cluster c.
 * @return cluster_t  It returns the resized cluster.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * This function merges 2 clusters together. It appends all the objects
 * of a cluster c2 to a cluster c1.
 * @brief This function merges 2 clusters together.
 * @pre c1 != NULL
 * @pre c2 != NULL
 * @param     c1  This parameter is a pointer to the cluster
 *                to which the objects will be added.
 * @param[in] c2  This parameter is a pointer to the cluster
 *                objects of which will be copied to cluster c1.
 * @post The clusters has been merged.
 * @return nothing
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * This function removes cluster at the index idx from the array
 * of clusters carr. It clears the given cluster and initialises
 * an empty cluster. Then it copies all the objects of the cluster
 * to the previous one, starting with that on the index idx.
 * If needed it also resizes the cluster to which the cluster will
 * be copied and sets size of the new cluster.
 * @brief This function removes a cluster.
 * @pre idx < narr
 * @pre narr > 0
 * @param     carr  This parameter is a pointer to the array of
 *                  clusters.
 * @param[in] narr  This parameter represents number of clusters
 *                  in the array.
 * @param[in] idx   This parameter represents index of cluster,
 *                  which will be removed.
 * @return    int   It returns new number of clusters.
 * @post The cluster has been removed.
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/** @} */

/** @defgroup Functions_finding_neighbours
 *  @brief The functions used for finding the nearest clusters.
 *  @{
 */

/**
 * This function uses Euclidean distance to calculate
 * distance between 2 objects.
 * @brief This function calculates distance between 2 objects.
 * @pre o1 != NULL
 * @pre o2 != NULL
 * @param[in] o1  This parameter represents the first object.
 * @param[in] o2  This parameter represents the second object.
 * @return    float It returns distance between 2 objects.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * This function calculates the smallest distance between all the
 * objects of 2 clusters.
 * @brief This function calculates distance between 2 clusters.
 * @pre c1 != NULL
 * @pre o1->size > 0
 * @pre c2 != NULL
 * @pre o2->size > 0
 * @param[in] c1    This parameter represents the first cluster.
 * @param[in] c2    This parameter represents the second cluster.
 * @return    float It returns distance between 2 clusters.
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * This function looks for 2 nearest clusters. In the array of
 * clusters carr of size narr it looks for 2 nearest clusters.
 * Each cluster is identified by its index. It saves indices of
 * 2 nearest clusters at the address pointed by c1 and c1.
 * To find 2 nearest clusters it uses smallest distance between
 * 2 clusters.
 * @brief This function finds 2 closest clusters.
 * @pre narr > 0
 * @param[in]  carr  This parameter represents array of clusters.
 * @param[in]  narr  This parameter represents a number of clusters
 *                   in the array.
 * @param[out] c1    This parameter represents index of the first cluster.
 * @param[out] c2    This parameter represents index of the second cluster.
 * @return     nothing
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/** @} */

/** @defgroup Functions_print
 *  @brief The functions used for printing clusters.
 *  @{
 */

/**
 * This function sorts objects in the cluster c due to their identifier.
 * @brief This function sorts objects.
 * @param[in]  c  This parameter represents cluster objects of
 *                which will be sorted.
 * @return     nothing
 */
void sort_cluster(struct cluster_t *c);

/**
 * @brief This function prints out cluster c using stdout.
 * @param[in]  c  This parameter represents cluster objects of
 *                which will be printed.
 * @return     nothing
 */
void print_cluster(struct cluster_t *c);

/** @} */

/** @defgroup Function_load
 *  @brief The functions used for loading clusters.
 *  @{
 */

/**
 * This function loads objects from file filename. For each object it creates
 * a cluster. It allocates memory for array of all the clusters
 * and it saves pointer to the first cluster to the address referred by
 * parameter arr. It returns a number of loaded clusters. In case of any
 * error it saves NULL to the address referred by parameter arr.
 * @brief This function loads the clusters.
 * @pre arr != NULL
 * @param[in]   filename  This parameter represents name of a file from which
 *                        the clusters will be loaded.
 * @param[out]  arr       This parameter represents pointer on array
 *                        of clusters which we be initiated and loaded.
 * @return      int       It returns a number of loaded clusters.
 * @post The file filename has been open.
 * @post Number of clusters was read from the file filename.
 * @post Memory for array of clusters has been allocated.
 * @post The clusters has been loaded corrected.
 * @post Coordinates of objects has been right.
 */
int load_clusters(char *filename, struct cluster_t **arr);

/** @} */

/** @addtogroup Functions_print
 *
 *  @{
 */

/**
 * This function prints out array of clusters carr of size narr one
 * by one using stdout.
 * @brief This function prints out clusters.
 * @param[in]  carr  This parameter represents array of clusters
 *                   which will be printed.
 * @param[in]  narr  This parameter represents a number of cluster
 *                   in the array
 * @return     nothing
 */
void print_clusters(struct cluster_t *carr, int narr);
/** @} */
