#include "grman/grman.h"
#include <iostream>
#include "graph.h"

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g;

    ///case1

    std::string image ;
    std::cout << " entrez le nom du graph a ouvrir : ";
    std::cin>>image;
    g.charger_graphe(image);

    ///case2
    std::string image ;
    std::cout << " entrez le nom du nouveau a graph : ";
    std::cin>>image;
    g.new_graph(image);

    /// supression arrête d'indice 1
    //g.supprimer_sommet(7);

    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {

        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
        g.ajouter_sommet();
        g.ajouter_arrete();
        g.supprimer_sommet();
        g.supprimer_arrete_choix();

    }
    g.update_graph(image);

    grman::fermer_allegro();

    return 0;

}
END_OF_MAIN();


