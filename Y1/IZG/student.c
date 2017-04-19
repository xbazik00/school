/******************************************************************************
 * Projekt - Zaklady pocitacove grafiky - IZG
 * spanel@fit.vutbr.cz
 *
 * $Id:xbazik00$
 */

#include "student.h"
#include "transform.h"
#include "fragment.h"

#include <memory.h>
#include <math.h>


/*****************************************************************************
 * Globalni promenne a konstanty
 */

/* Typ/ID rendereru (nemenit) */
const int           STUDENT_RENDERER = 1;

/* Globalne premenne pre ziskanie upraveneho casu */
float n=0;
int prev_tick=0;


/*****************************************************************************
 * Funkce vytvori vas renderer a nainicializuje jej
 */

S_Renderer * studrenCreate()
{
    S_StudentRenderer * renderer = (S_StudentRenderer *)malloc(sizeof(S_StudentRenderer));
    IZG_CHECK(renderer, "Cannot allocate enough memory");

    /* inicializace default rendereru */
    renderer->base.type = STUDENT_RENDERER;
    renInit(&renderer->base);

    /* nastaveni ukazatelu na upravene funkce */
    /* napr. renderer->base.releaseFunc = studrenRelease; */
    /* ??? */

    renderer->base.releaseFunc = studrenRelease;
    renderer->base.projectTriangleFunc = studrenProjectTriangle;
    

    /* inicializace nove pridanych casti */
    /* ??? */
    renderer->text = loadBitmap(TEXTURE_FILENAME,&renderer->sirka,&renderer->vyska);


    return (S_Renderer *)renderer;
}

/*****************************************************************************
 * Funkce korektne zrusi renderer a uvolni pamet
 */

void studrenRelease(S_Renderer **ppRenderer)
{
    S_StudentRenderer * renderer;

    if( ppRenderer && *ppRenderer )
    {
        /* ukazatel na studentsky renderer */
        renderer = (S_StudentRenderer *)(*ppRenderer);

        /* pripadne uvolneni pameti */
        /* ??? */
		/* Vycistenie textury */
        free(renderer->text);

        /* fce default rendereru */
        renRelease(ppRenderer);
    }
}

/******************************************************************************
 * Nova fce pro rasterizaci trojuhelniku s podporou texturovani
 * Upravte tak, aby se trojuhelnik kreslil s texturami
 * (doplnte i potrebne parametry funkce - texturovaci souradnice, ...)
 * v1, v2, v3 - ukazatele na vrcholy trojuhelniku ve 3D pred projekci
 * n1, n2, n3 - ukazatele na normaly ve vrcholech ve 3D pred projekci
 * x1, y1, ... - vrcholy trojuhelniku po projekci do roviny obrazovky
 */

void studrenDrawTriangle(S_Renderer *pRenderer,
                         S_Coords *v1, S_Coords *v2, S_Coords *v3,
                         S_Coords *n1, S_Coords *n2, S_Coords *n3,
                         int x1, int y1,
                         int x2, int y2,
                         int x3, int y3,
                         S_Coords *t1,S_Coords *t2,S_Coords *t3,
                         double ah, double bh, double ch 
                         )
{
    /* zaklad fce zkopirujte z render.c */
    /* ??? */


    int         minx, miny, maxx, maxy;
    int         a1, a2, a3, b1, b2, b3, c1, c2, c3;
    int         s1, s2, s3;
    int         x, y, e1, e2, e3;
    double      alpha, beta, gamma, w1, w2, w3, z, tx,ty;
    S_RGBA      col1, col2, col3, color, get_color;

    IZG_ASSERT(pRenderer && v1 && v2 && v3 && n1 && n2 && n3);

    /* vypocet barev ve vrcholech */
    col1 = pRenderer->calcReflectanceFunc(pRenderer, v1, n1);
    col2 = pRenderer->calcReflectanceFunc(pRenderer, v2, n2);
    col3 = pRenderer->calcReflectanceFunc(pRenderer, v3, n3);

    /* obalka trojuhleniku */
    minx = MIN(x1, MIN(x2, x3));
    maxx = MAX(x1, MAX(x2, x3));
    miny = MIN(y1, MIN(y2, y3));
    maxy = MAX(y1, MAX(y2, y3));

    /* oriznuti podle rozmeru okna */
    miny = MAX(miny, 0);
    maxy = MIN(maxy, pRenderer->frame_h - 1);
    minx = MAX(minx, 0);
    maxx = MIN(maxx, pRenderer->frame_w - 1);

    /* Pineduv alg. rasterizace troj.
       hranova fce je obecna rovnice primky Ax + By + C = 0
       primku prochazejici body (x1, y1) a (x2, y2) urcime jako
       (y1 - y2)x + (x2 - x1)y + x1y2 - x2y1 = 0 */

    /* normala primek - vektor kolmy k vektoru mezi dvema vrcholy, tedy (-dy, dx) */
    a1 = y1 - y2;
    a2 = y2 - y3;
    a3 = y3 - y1;
    b1 = x2 - x1;
    b2 = x3 - x2;
    b3 = x1 - x3;

    /* koeficient C */
    c1 = x1 * y2 - x2 * y1;
    c2 = x2 * y3 - x3 * y2;
    c3 = x3 * y1 - x1 * y3;

    /* vypocet hranove fce (vzdalenost od primky) pro protejsi body */
    s1 = a1 * x3 + b1 * y3 + c1;
    s2 = a2 * x1 + b2 * y1 + c2;
    s3 = a3 * x2 + b3 * y2 + c3;

    if ( !s1 || !s2 || !s3 )
    {
        return;
    }

    /* normalizace, aby vzdalenost od primky byla kladna uvnitr trojuhelniku */
    if( s1 < 0 )
    {
        a1 *= -1;
        b1 *= -1;
        c1 *= -1;
    }
    if( s2 < 0 )
    {
        a2 *= -1;
        b2 *= -1;
        c2 *= -1;
    }
    if( s3 < 0 )
    {
        a3 *= -1;
        b3 *= -1;
        c3 *= -1;
    }

    /* koeficienty pro barycentricke souradnice */
    alpha = 1.0 / ABS(s2);
    beta = 1.0 / ABS(s3);
    gamma = 1.0 / ABS(s1);

    /* vyplnovani... */
    for( y = miny; y <= maxy; ++y )
    {
        /* inicilizace hranove fce v bode (minx, y) */
        e1 = a1 * minx + b1 * y + c1;
        e2 = a2 * minx + b2 * y + c2;
        e3 = a3 * minx + b3 * y + c3;

        for( x = minx; x <= maxx; ++x )
        {
            if( e1 >= 0 && e2 >= 0 && e3 >= 0 )
            {
                /* interpolace pomoci barycentrickych souradnic
                   e1, e2, e3 je aktualni vzdalenost bodu (x, y) od primek */
                w1 = alpha * e2;
                w2 = beta * e3;
                w3 = gamma * e1;

                /* interpolace z-souradnice a texturovacich suradnic*/
                z = w1 * v1->z + w2 * v2->z + w3 * v3->z;
                tx = (w1 * t1->x/ah + w2 * t2->x/bh + w3 * t3->x/ch)/(w1/ah + w2/bh + w3/ch);
                ty = (w1 * t1->y/ah + w2 * t2->y/bh + w3 * t3->y/ch)/(w1/ah + w2/bh + w3/ch);
/*
                tx = w1 * t1->x + w2 * t2->x + w3 * t3->x;
                ty = w1 * t1->y + w2 * t2->y + w3 * t3->y;*/

                get_color = studrenTextureValue((S_StudentRenderer *)pRenderer, tx, ty);


                /* interpolace barvy */
                color.red = ROUND2BYTE(w1 * col1.red + w2 * col2.red + w3 * col3.red)*(get_color.red/255.0);
                color.green = ROUND2BYTE(w1 * col1.green + w2 * col2.green + w3 * col3.green)*(get_color.green/255.0);
                color.blue = ROUND2BYTE(w1 * col1.blue + w2 * col2.blue + w3 * col3.blue)*(get_color.blue/255.0);
                color.alpha = 255;

                /* vykresleni bodu */
                if( z < DEPTH(pRenderer, x, y) )
                {
                    PIXEL(pRenderer, x, y) = color;
                    DEPTH(pRenderer, x, y) = z;
                }
            }

            /* hranova fce o pixel vedle */
            e1 += a1;
            e2 += a2;
            e3 += a3;
        }
    }
}

/******************************************************************************
 * Vykresli i-ty trojuhelnik n-teho klicoveho snimku modelu
 * pomoci nove fce studrenDrawTriangle()
 * Pred vykreslenim aplikuje na vrcholy a normaly trojuhelniku
 * aktualne nastavene transformacni matice!
 * Upravte tak, aby se model vykreslil interpolovane dle parametru n
 * (cela cast n udava klicovy snimek, desetinna cast n parametr interpolace
 * mezi snimkem n a n + 1)
 * i - index trojuhelniku
 * n - index klicoveho snimku (float pro pozdejsi interpolaci mezi snimky)
 */

void studrenProjectTriangle(S_Renderer *pRenderer, S_Model *pModel, int i, float n)
{
    /* zaklad fce zkopirujte z render.c */
    /* ??? */

    S_Coords    aa, bb, cc;             /* souradnice vrcholu po transformaci */
    S_Coords    naa, nbb, ncc;          /* normaly ve vrcholech po transformaci */
    S_Coords    nn;                     /* normala trojuhelniku po transformaci */
    int         u1, v1, u2, v2, u3, v3; /* souradnice vrcholu po projekci do roviny obrazovky */
    S_Triangle  * triangle;
    int         vertexOffset, normalOffset; /* offset pro vrcholy a normalove vektory trojuhelniku */
    int         vertexOffset2, normalOffset2;
    int         i0, i1, i2, in;             /* indexy vrcholu a normaly pro i-ty trojuhelnik n-teho snimku */
    int         i02, i12, i22, in2;
    double      ah,bh,ch;

    IZG_ASSERT(pRenderer && pModel && i >= 0 && i < trivecSize(pModel->triangles) && n >= 0 );

    /* z modelu si vytahneme i-ty trojuhelnik */
    triangle = trivecGetPtr(pModel->triangles, i);

    /* ziskame offset pro vrcholy n-teho snimku */
    vertexOffset = (((int) n) % pModel->frames) * pModel->verticesPerFrame;

    /* ziskame offset pro normaly trojuhelniku n-teho snimku */
    normalOffset = (((int) n) % pModel->frames) * pModel->triangles->size;

    /* ziskame offset pro vrcholy n+1-teho snimku */
    vertexOffset2 = (((int) n+1) % pModel->frames) * pModel->verticesPerFrame;

    /* ziskame offset pro normaly trojuhelniku n+1-teho snimku */
    normalOffset2 = (((int) n+1) % pModel->frames) * pModel->triangles->size;

    /* indexy vrcholu pro i-ty trojuhelnik n-teho snimku - pricteni offsetu */
    i0 = triangle->v[ 0 ] + vertexOffset;
    i1 = triangle->v[ 1 ] + vertexOffset;
    i2 = triangle->v[ 2 ] + vertexOffset;

    /* indexy vrcholu pro i-ty trojuhelnik n+1-teho snimku - pricteni offsetu */
    i02 = triangle->v[ 0 ] + vertexOffset2;
    i12 = triangle->v[ 1 ] + vertexOffset2;
    i22 = triangle->v[ 2 ] + vertexOffset2;

    /* index normaloveho vektoru pro i-ty trojuhelnik n-teho snimku - pricteni offsetu */
    in = triangle->n + normalOffset;

    /* index normaloveho vektoru pro i-ty trojuhelnik n+1-teho snimku - pricteni offsetu */
    in2 = triangle->n + normalOffset2;

	/* Body pre linearnu interpolaciu */
    S_Coords* xx = cvecGetPtr(pModel->vertices, i0);
    S_Coords* yy = cvecGetPtr(pModel->vertices, i1);
    S_Coords* zz = cvecGetPtr(pModel->vertices, i2);
    
    S_Coords* xx2 = cvecGetPtr(pModel->vertices, i02);
    S_Coords* yy2 = cvecGetPtr(pModel->vertices, i12);
    S_Coords* zz2 = cvecGetPtr(pModel->vertices, i22);

    S_Coords* xxn = cvecGetPtr(pModel->normals, i0);
    S_Coords* yyn = cvecGetPtr(pModel->normals, i1);
    S_Coords* zzn = cvecGetPtr(pModel->normals, i2);
    
    S_Coords* xxn2 = cvecGetPtr(pModel->normals, i02);
    S_Coords* yyn2 = cvecGetPtr(pModel->normals, i12);
    S_Coords* zzn2 = cvecGetPtr(pModel->normals, i22);

    S_Coords* nv = cvecGetPtr(pModel->trinormals, in);
    S_Coords* nv2 = cvecGetPtr(pModel->trinormals, in2);


    double x = n;
    double integral;
    double fractional = modf(x, &integral);
    

    S_Coords nx,ny,nz,nnv,nxn,nyn,nzn;

	/* Linearna interpolacia bodov */
    nx.x = (1-fractional)*xx->x + fractional*xx2->x;
    nx.y = (1-fractional)*xx->y + fractional*xx2->y;
    nx.z = (1-fractional)*xx->z + fractional*xx2->z;

    ny.x = (1-fractional)*yy->x + fractional*yy2->x;
    ny.y = (1-fractional)*yy->y + fractional*yy2->y;
    ny.z = (1-fractional)*yy->z + fractional*yy2->z;

    nz.x = (1-fractional)*zz->x + fractional*zz2->x;
    nz.y = (1-fractional)*zz->y + fractional*zz2->y;
    nz.z = (1-fractional)*zz->z + fractional*zz2->z;

    nxn.x = (1-fractional)*xxn->x + fractional*xxn2->x;
    nxn.y = (1-fractional)*xxn->y + fractional*xxn2->y;
    nxn.z = (1-fractional)*xxn->z + fractional*xxn2->z;

    nyn.x = (1-fractional)*yyn->x + fractional*yyn2->x;
    nyn.y = (1-fractional)*yyn->y + fractional*yyn2->y;
    nyn.z = (1-fractional)*yyn->z + fractional*yyn2->z;

    nzn.x = (1-fractional)*zzn->x + fractional*zzn2->x;
    nzn.y = (1-fractional)*zzn->y + fractional*zzn2->y;
    nzn.z = (1-fractional)*zzn->z + fractional*zzn2->z;

    nnv.x = (1-fractional)*nv->x + fractional*nv2->x;
    nnv.y = (1-fractional)*nv->y + fractional*nv2->y;
    nnv.z = (1-fractional)*nv->z + fractional*nv2->z;
    


    /* transformace vrcholu matici model */
    trTransformVertex(&aa, &nx);
    trTransformVertex(&bb, &ny);
    trTransformVertex(&cc, &nz);

    /* promitneme vrcholy trojuhelniku na obrazovku */
    ah=trProjectVertex(&u1, &v1, &aa);
    bh=trProjectVertex(&u2, &v2, &bb);
    ch=trProjectVertex(&u3, &v3, &cc);

    /* pro osvetlovaci model transformujeme take normaly ve vrcholech */
    trTransformVector(&naa, &nxn);
    trTransformVector(&nbb, &nyn);
    trTransformVector(&ncc, &nzn);

    /* normalizace normal */
    coordsNormalize(&naa);
    coordsNormalize(&nbb);
    coordsNormalize(&ncc);

    /* transformace normaly trojuhelniku matici model */
    trTransformVector(&nn, &nnv);
    
    /* normalizace normaly */
    coordsNormalize(&nn);

    /* je troj. privraceny ke kamere, tudiz viditelny? */
    if( !renCalcVisibility(pRenderer, &aa, &nn) )
    {
        /* odvracene troj. vubec nekreslime */
        return;
    }

    /* rasterizace trojuhelniku */
    studrenDrawTriangle(pRenderer,
                    &aa, &bb, &cc,
                    &naa, &nbb, &ncc,
                    u1, v1, u2, v2, u3, v3,
                    &triangle->t[0],&triangle->t[1],&triangle->t[2],
                    ah,bh,ch
                    );

}

/******************************************************************************
* Vraci hodnotu v aktualne nastavene texture na zadanych
* texturovacich souradnicich u, v
* Pro urceni hodnoty pouziva bilinearni interpolaci
* Pro otestovani vraci ve vychozim stavu barevnou sachovnici dle uv souradnic
* u, v - texturovaci souradnice v intervalu 0..1, ktery odpovida sirce/vysce textury
*/

S_RGBA studrenTextureValue( S_StudentRenderer * pRenderer, double u, double v )
{
    /* ??? */

    //unsigned char c = ROUND2BYTE( ( ( fmod( u * 10.0, 1.0 ) > 0.5 ) ^ ( fmod( v * 10.0, 1.0 ) < 0.5 ) ) * 255 );
    S_RGBA col,col2;

    int text_size = pRenderer->sirka;
    u=u*(text_size-1);
    v=v*(text_size-1);
	
	/* Ziskanie najblizsich bodov */
    int u_up = (int)ceil(u);
    int u_low = (int)floor(u);
    int v_up = (int)ceil(v);
    int v_low = (int)floor(v);

	/* Inicializacia farieb na 0 */
	col.red = 0;
	col.green = 0;
	col.blue = 0;


	/* Bilinearna interpolacia farieb */
	col.red =  pRenderer->text[u_low * text_size + v_low].red * (u_up-u);
    col.red += pRenderer->text[u_low * text_size + v_up].red * (u - u_low);
    col2.red = pRenderer->text[u_up * text_size + v_low].red * (u_up - u);
    col2.red += pRenderer->text[u_up * text_size + v_up].red * (u - u_low);
	col.red = col2.red * (v_up - v)  + col.red * (v - v_low);

	col.green =  pRenderer->text[u_low * text_size + v_low].green * (u_up-u);
    col.green += pRenderer->text[u_low * text_size + v_up].green * (u - u_low);
    col2.green = pRenderer->text[u_up * text_size + v_low].green * (u_up - u);
    col2.green += pRenderer->text[u_up * text_size + v_up].green * (u - u_low);
	col.green = col2.green * (v_up - v)  + col.green * (v - v_low);
		
	col.blue =  pRenderer->text[u_low * text_size + v_low].blue * (u_up-u);
    col.blue += pRenderer->text[u_low * text_size + v_up].blue * (u - u_low);
    col2.blue = pRenderer->text[u_up * text_size + v_low].blue * (u_up - u);
    col2.blue += pRenderer->text[u_up * text_size + v_up].blue * (u - u_low);
	col.blue = col2.blue * (v_up - v)  + col.blue * (v - v_low);

/*
    col.red =  pRenderer->text[ui * text_size + vi].red;
    col.green =  pRenderer->text[ui * text_size + vi].green;
    col.blue =  pRenderer->text[ui * text_size + vi].blue;*/

    col.alpha = 255;
    return makeColor( fmod(col.red, 255.0), fmod(col.green,255.0),  fmod(col.blue,255.0 ));
    //return makeColor( c, 255 - c, 0 );
    

}

/******************************************************************************
 ******************************************************************************
 * Funkce pro vyrenderovani sceny, tj. vykresleni modelu
 * Upravte tak, aby se model vykreslil animovane
 * (volani renderModel s aktualizovanym parametrem n)
 */

void renderStudentScene(S_Renderer *pRenderer, S_Model *pModel)
{
    /* zaklad fce zkopirujte z main.c */
    /* ??? */
    const S_Material    MAT_WHITE_AMBIENT = { 1.0, 1.0, 1.0, 1.0 };
    const S_Material    MAT_WHITE_DIFFUSE = { 1.0, 1.0, 1.0, 1.0 };
    const S_Material    MAT_WHITE_SPECULAR = { 1.0, 1.0, 1.0, 1.0 };

    /* test existence frame bufferu a modelu */
    IZG_ASSERT(pModel && pRenderer);

    /* nastavit projekcni matici */
    trProjectionPerspective(pRenderer->camera_dist, pRenderer->frame_w, pRenderer->frame_h);

    /* vycistit model matici */
    trLoadIdentity();

    /* nejprve nastavime posuv cele sceny od/ke kamere */
    trTranslate(0.0, 0.0, pRenderer->scene_move_z);

    /* nejprve nastavime posuv cele sceny v rovine XY */
    trTranslate(pRenderer->scene_move_x, pRenderer->scene_move_y, 0.0);

    /* natoceni cele sceny - jen ve dvou smerech - mys je jen 2D... :( */
    trRotateX(pRenderer->scene_rot_x);
    trRotateY(pRenderer->scene_rot_y);

    /* nastavime material */
    renMatAmbient(pRenderer, &MAT_WHITE_AMBIENT);
    renMatDiffuse(pRenderer, &MAT_WHITE_DIFFUSE);
    renMatSpecular(pRenderer, &MAT_WHITE_SPECULAR);

    /* a vykreslim nas model (ve vychozim stavu kreslime pouze snimek 0) */
    renderModel(pRenderer, pModel, n);
}

/* Callback funkce volana pri tiknuti casovace
 * ticks - pocet milisekund od inicializace */
void onTimer( int ticks )
{
    /* uprava parametru pouzivaneho pro vyber klicoveho snimku
     * a pro interpolaci mezi snimky */
    /* ??? */
    int val = ticks - prev_tick;
	
	n+=(float)((double)(val)/100.0);
	prev_tick=ticks;
}

/*****************************************************************************
 *****************************************************************************/
