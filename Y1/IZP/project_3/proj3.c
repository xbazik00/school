/**
 * Kostra programu pro 3. projekt IZP 2015/16
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 * http://is.muni.cz/th/172767/fi_b/5739129/web/web/slsrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h>

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    c->obj=malloc(cap*sizeof(struct obj_t));
    c->capacity = cap;
    if(c->obj==NULL)
        c->capacity=0;
    // TODO

}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    free(c->obj);
    init_cluster(c,0);
    c->size = 0;
    // TODO
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{

    if (c->size==c->capacity)
    {
        resize_cluster(c,c->capacity+1);
    }

    c->obj[c->size] = obj;
    c->size+=1;
    // TODO
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    for (int i = 0; i<c2->size;i++)
    {
         append_cluster(c1,c2->obj[i]);
    }
    // TODO
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    clear_cluster(carr);                                       //clears the given cluster
    for (int i = 0;i<narr-idx-1;i++)
    {
        if(carr[i].capacity<carr[i+1].capacity)                //if needed resizes capacity of the cluster
        {
            resize_cluster(&carr[i],carr[i+1].capacity);
        }
        for(int j = 0; j<carr[i+1].size;j++)
        {
            carr[i].obj[j].id=carr[i+1].obj[j].id;
            carr[i].obj[j].x=carr[i+1].obj[j].x;
            carr[i].obj[j].y=carr[i+1].obj[j].y;
        }
        carr[i].size=carr[i+1].size;
    }
    return narr-1;
    // TODO
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    float dist;
    dist = sqrtf(pow(o1->x-o2->x,2)+pow(o1->y-o2->y,2));               //formula for Euclidean distance
    return dist;
    // TODO
}

/*
 Pocita vzdalenost dvou shluku. Vzdalenost je vypoctena na zaklade nejblizsiho
 souseda.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    float dist = obj_distance(&c1->obj[0],&c2->obj[0]);                  //distance of the first objects of two clusters
    for(int i = 0; i < c1->size;i++)                                     //looks for 2 objects with the smallest distance from each other
    {
        for (int j = 0; j<c2->size;j++)
        {
             if(dist>obj_distance(&c1->obj[i],&c2->obj[j]))
             {
                  dist = obj_distance(&c1->obj[i],&c2->obj[j]);
             }
        }
    }
    return dist;
    // TODO
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky (podle nejblizsiho souseda). Nalezene shluky
 identifikuje jejich indexy v poli 'carr'. Funkce nalezene shluky (indexy do
 pole 'carr') uklada do pameti na adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    float dist = cluster_distance(&carr[0],&carr[1]);              //calculates distance of the first 2 clusters
    *c1=0;
    *c2=1;

    for(int i = 0; i<narr;i++)
    {
        for(int j = i+1; j<narr; j++)
        {
            if(dist>cluster_distance(&carr[i],&carr[j]))           //compares with the previous smallest distance
            {
                *c1=i;
                *c2=j;
                dist = cluster_distance(&carr[i],&carr[j]);
            }
        }
    }

    // TODO
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = a;
    const struct obj_t *o2 = b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);
    int count = 1;
    int objid;
    float x,y;
    FILE *fp=fopen(filename,"rt");

    if(fp==NULL)                                                        //checks whether the file was open
    {
        fprintf(stderr,"Error: File was not open.\n");
        arr=NULL;
        return 0;
    }

    if(fscanf(fp,"count=%d",&count)!=1)                                    //number of clusters in input text file
    {
        fprintf(stderr,"Error: Number of clusters was not found in the input file.\n");
        arr=NULL;
        fclose(fp);
        return 0;
    }
    struct cluster_t *AoC = malloc(count*sizeof(struct cluster_t));     //allocates memory for array of clusters
    if (AoC==NULL)
    {
        fprintf(stderr,"Error: Memory for array of clusters was not allocated.\n");
        arr=NULL;
        fclose(fp);
        return 0;
    }
    for(int i =0;i<count;i++)                                           //initialisation of objects
    {
        init_cluster (&AoC[i],CLUSTER_CHUNK);

        if (fscanf(fp,"%d %f %f",&objid,&x,&y)!=3)
        {
            fprintf(stderr,"Error: Invalid input.\n");
            arr=NULL;
            for(int j = 0;j<=i;j++)
            {
                 free(AoC[j].obj);
            }
            free(AoC);
            fclose(fp);
            return 0;
        }
        if (x<0||x>1000||y<0||y>1000)
        {
            fprintf(stderr,"Error: Invalid input.\n");
            arr=NULL;
            for(int j = 0;j<=i;j++)
            {
                 free(AoC[j].obj);
            }
            free(AoC);
            fclose(fp);
            return 0;
        }
        else
        {
            AoC[i].obj->id=objid;
            AoC[i].obj->x=x;
            AoC[i].obj->y=y;
            AoC[i].size=1;
        }
    }
    *arr=&AoC[0];
    fclose(fp);
    return count;
    // TODO
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    char *ptr;
    int number = 1;
    if (argc>2)                                        //if at least 2 arguments were given
    {
        long int n = strtol(argv[2],&ptr,10);
        if (!strcmp(ptr,"")&&n>0&&n<=INT_MAX)          //checks if the second argument is correct
        {
            number = (int)n;
        }

        else
        {
             fprintf(stderr,"Error: Invalid input.\n");
             return 1;
        }
    }
    if (argc==1)
    {
        fprintf(stderr,"Error: Invalid input.\n");
        return 1;
    }
    struct cluster_t *clusters;
    int count;
    int first,second;

    count = load_clusters(argv[1],&clusters);
    if (count==0)
    {
        return 1;
    }
    if(count>=number)
    {
        for (int i = 0;i<count-number;i++)
        {
            find_neighbours(clusters,(count-i),&first,&second);
            merge_clusters(&clusters[first],&clusters[second]);
            remove_cluster(&clusters[second],(count-i),second);
        }

        for (int i = 0; i<count-number;i++)
        {
            sort_cluster(&clusters[i]);
        }

        print_clusters(clusters,number);
        for(int i = 0;i<count;i++)
        {
            free(clusters[i].obj);
        }
        free(clusters);
        return 0;
    }
    else
    {
        print_clusters(clusters,count);
        for (int i =0;i<count;i++)
        {
            free(clusters[i].obj);
        }
        free(clusters);
        return 0;
    }
    // TODO
}
