#include "graph.h"
#include <fstream>
#include <iostream>

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}


/// Gestion du Vertex avant l'appel � l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel � l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}


/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);
}


/// M�thode sp�ciale qui construit un graphe arbitraire (d�mo)
/// Cette m�thode est � enlever et remplacer par un syst�me
/// de chargement de fichiers par exemple.
/// Bien s�r on ne veut pas que vos graphes soient construits
/// "� la main" dans le code comme �a.
void Graph::charger_graphe(std::string btp)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne pr�c�dente est en gros �quivalente � :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent �tre d�finis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    std::string name = btp+"/image.txt";
    std::ifstream fichier2(name);


    //Ouverture du fichier en lecture
    if(fichier2)
    {
        //Initialisation de l'ordre et du nbr d'aretes
        //int m_img;
        int indice1, indice2, indice3, indice5;
        double indice4;
        std::string nom;
        //fichier2>>m_img;
        int nbLignes =0;
        std::string ligne;
        while(std::getline(fichier2, ligne))
        {
            nbLignes++;
        }

        fichier2.close();

        std::ifstream fichier2(name);


        // serait automatiquement ferm� � la sortie du bloc
        //Initialisation du tableau d'aretes


        for(int i=0; i< nbLignes; i++)
        {
            //Lecture des indices de sommets de chaque arete � partir du fichier, puis affectation dans le tableau
            //D'aretes du graphe


            fichier2>>indice1>>indice4>>indice2>>indice3>>nom>>indice5;

            add_interfaced_vertex(indice1, indice4, indice2, indice3, nom,indice5);


        }
        fichier2.close();


    }




    /// Les arcs doivent �tre d�finis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...

    std::string nameA = btp+"/arc.txt";

    std::ifstream fichier(nameA);


    //Ouverture du fichier en lecture
    if(fichier)
    {
        int nbLignesB =0;
        std::string ligneB;
        while(std::getline(fichier, ligneB))
        {
            nbLignesB++;
        }

        fichier.close();

        std::ifstream fichier(nameA);


        //Initialisation du tableau d'aretes
        for(int i=0; i<nbLignesB; i++)
        {
            //Lecture des indices de sommets de chaque arete � partir du fichier, puis affectation dans le tableau
            //D'aretes du graphe
            int indice1, indice2, indice3;
            double poids;
            fichier>>indice1>>indice2>>indice3>>poids;

            add_interfaced_edge(indice1, indice2, indice3, poids);


        }

        fichier.close();

    }

    else
    {
        std::cout<<"Error"<<std::endl;
    }
}
void Graph::new_graph(std::string image)
{

    std::string const fichier(image);
    mkdir(image.c_str());

    std::string emplacementA;
    std::string emplacementB;

    emplacementA = image + "/image.txt";
    emplacementB = image + "/arc.txt";
    std::string const fic (emplacementA);
    std::string const fib (emplacementB);

    std::ofstream Flux(fic.c_str());
    std::ofstream Fluxp(fib.c_str());

    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);


}

void Graph::update_graph(std:: string btp)
{
    std::string image = btp + "/image.txt";
    std::ofstream fichier2(image);

    int indice2, indice4;
    double indice1;
    std::string nom;
    //Ouverture du fichier en lecture + �criture
    if(fichier2)
    {


        for(auto &elem : m_vertices)
        {
            int Pos_x= elem.second.m_interface->m_top_box.get_frame().pos.x;
            int Pos_y=elem.second.m_interface->m_top_box.get_frame().pos.y;
            indice2= elem.second.m_value;
            indice1= elem.first;
            nom = elem.second.m_interface->m_img.m_pic_name;
            fichier2<<indice1<<" "<<indice2<<" "<< Pos_x <<" " <<  Pos_y << " "<< nom <<" "<<"0"<< std::endl;
        }

    }

    else
    {
        std::cout<<"Error"<<std::endl;
    }

    fichier2.close();

    std::string image2 = btp + "/arc.txt";
    std::ofstream fichier3(image2);
     if(fichier3)
    {


        for(auto &elem : m_edges)
        {
            int sommet_A= elem.second.m_from;
            int sommet_B= elem.second.m_to;

            fichier3<<elem.first<<" "<<elem.second.m_from<<" " << elem.second.m_to<<" "<<elem.second.m_weight<< std::endl;
        }
    }

     fichier3.close();


}

/* void Graph::update_arc(std::string btp)
{
    std::string image = btp + "/arc.txt";
    std::ofstream fichier2(image);

    int indice2, indice4;
    double indice1;
    std::string nom;
    //Ouverture du fichier en lecture + �criture
    if(fichier2)
    {


        for(auto &elem : m_vertices)
        {
            int Pos_x= elem.second.m_interface->m_top_box.get_frame().pos.x;
            int Pos_y=elem.second.m_interface->m_top_box.get_frame().pos.y;
            indice2= elem.second.m_value;
            indice1= elem.first;
            nom = elem.second.m_interface->m_img.m_pic_name;
            fichier2<<indice1<<" "<<indice2<<" "<< Pos_x <<" " <<  Pos_y << " "<< nom <<" "<<"0"<< std::endl;
        }

    }

    else
    {
        std::cout<<"Error"<<std::endl;
    }

    fichier2.close();

}
*/

/*void Graph::creation()
{

    int indice1, indice2, indice3, indice5;
    double indice4;
    std::string nom;
    int choix;
    std::cin>>choix>>std::endl;

    //fichier2>>m_img;


    //Initialisation du tableau d'aretes
    for(int i=0; i<5; i++)
    {
        //Lecture des indices de sommets de chaque arete � partir du fichier, puis affectation dans le tableau
        //D'aretes du graphe


        fichier2>>indice1>>indice4>>indice2>>indice3>>nom>>indice5;

        add_interfaced_vertex(indice1, indice4, indice2, indice3, nom,indice5);


    }

    fichier2.close();
} */





/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

}

/// Aide � l'ajout de sommets interfac�s
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Cr�ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);


    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);


}

/// Aide � l'ajout d'arcs interfac�s
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);

    /// OOOPS ! Prendre en compte l'arc ajout� dans la topologie du graphe !
    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;

    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);

}

/// eidx index of edge to remove
void Graph::supprimer_arrete(int eidx)
{
    /// r�f�rence vers le Edge � enlever
    Edge &remed=m_edges.at(eidx);

    std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;

    /// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;

    /// test : on a bien des �l�ments interfac�s
    if (m_interface && remed.m_interface)
    {
        /// Ne pas oublier qu'on a fait �a � l'ajout de l'arc :
        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
        /* m_interface->m_main_box.add_child(ei->m_top_edge);  */
        /* m_edges[idx] = Edge(weight, ei); */
        /// Le new EdgeInterface ne n�cessite pas de delete car on a un shared_ptr
        /// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
        /// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }

    /// Il reste encore � virer l'arc supprim� de la liste des entrants et sortants des 2 sommets to et from !
    /// References sur les listes de edges des sommets from et to
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );

    /// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
    /// Il suffit donc de supprimer l'entr�e de la map pour supprimer � la fois l'Edge et le EdgeInterface
    /// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
    m_edges.erase( eidx );


    /// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;

}
void Graph::supprimer_arrete_choix()
{
    int eidx;
    if ( key[KEY_W])
    {
    std::cout<<" quel arrete voulez vous enlever ? : "<<std::endl;
    std::cin>>eidx;

    /// r�f�rence vers le Edge � enlever
    Edge &remed=m_edges.at(eidx);

    std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;

    /// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;

    /// test : on a bien des �l�ments interfac�s
    if (m_interface && remed.m_interface)
    {
        /// Ne pas oublier qu'on a fait �a � l'ajout de l'arc :
        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
        /* m_interface->m_main_box.add_child(ei->m_top_edge);  */
        /* m_edges[idx] = Edge(weight, ei); */
        /// Le new EdgeInterface ne n�cessite pas de delete car on a un shared_ptr
        /// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
        /// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }

    /// Il reste encore � virer l'arc supprim� de la liste des entrants et sortants des 2 sommets to et from !
    /// References sur les listes de edges des sommets from et to
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );

    /// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
    /// Il suffit donc de supprimer l'entr�e de la map pour supprimer � la fois l'Edge et le EdgeInterface
    /// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
    m_edges.erase( eidx );


    /// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;

    //m_edges[eidx].first=m_edges[eidx-1].first;
    }

}

void Graph::supprimer_sommet()
{
    /// r�f�rence vers le Edge � enlever
    int eidx;
    if (key[KEY_Q])
    {

    std::cout<<" quel sommet voulez vous enlecer ? : "<<std::endl;
    std::cin>>eidx;
    Vertex &remed=m_vertices.at(eidx);

    /// test : on a bien des �l�ments interfac�s
    if (m_interface && remed.m_interface)
    {
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_box );

        for(int i=0; i< m_vertices[eidx].m_in.size(); i++)
        {
            if (!m_vertices[eidx].m_in.empty())
            {
                supprimer_arrete(m_vertices[eidx].m_in[i]);
            }
        }
        for(int i=0; i< m_vertices[eidx].m_out.size(); i++)
        {
            if (!m_vertices[eidx].m_out.empty())
            {
                supprimer_arrete(m_vertices[eidx].m_out[i]);
            }
        }
        m_vertices.erase( eidx );
        }
    }
}

void Graph::ajouter_sommet()
{
    int i_sommet, x1, y1;
    double p_sommet;
    std::string n_sommet;

    if(mouse_b & 2)
    {
        x1 = mouse_x;
        y1 = mouse_y;

        std::cout << "Nouveau sommet" << std::endl;
        std::cout << "Entrer le nom du sommet" << std::endl;
        std::cin >> n_sommet;
        //n_sommet = n_sommet + ".new";
        std::cout << "Entrer la valeur du sommet" << std::endl;
        std::cin >> p_sommet;
        i_sommet = m_vertices.size()+1;
        add_interfaced_vertex(i_sommet,p_sommet,x1,y1,n_sommet,0);
//        int idx, double value, int x, int y, std::string pic_name, int pic_idx
        }
    }


void Graph::ajouter_arrete()
{
    int i_arete, cpt,sommet_dep,sommet_arr;
    double p_arete;
    std::string nom_image, n_sommet;

    if(key[KEY_SPACE] )
    {


        std::cout << "Combien d'aretes?" << std::endl;
        std::cin >> cpt;

        for(int i=0; i<cpt; i++)
        {
            i_arete = m_edges.size() + i +1;

            std::cout << "Arete numero " << i_arete << std::endl;
            std::cout << "Sommet de depart ?" << std::endl;
            std::cin >>sommet_dep;
            std::cout <<"sommet d'arriv� ?"<<std::endl;
            std::cin >>sommet_arr;
            std:: cout << "le poids ?"<<std::endl;
            std::cin >>p_arete;
            add_interfaced_edge(i_arete,sommet_dep,sommet_arr,p_arete);
        }
    }
}
/*
    /// Il reste encore � virer l'arc supprim� de la liste des entrants et sortants des 2 sommets to et from !
    /// References sur les listes de edges des sommets from et to
    std::vector<int> &ves = m_vertices[remed.m_in].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
    ves.erase( std::remove( ves.begin(), ves.end(), eidx ), ves.end() );

    /// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
    /// Il suffit donc de supprimer l'entr�e de la map pour supprimer � la fois l'Edge et le EdgeInterface
    /// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
    m_vertices.erase( eidx );


    /// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl; */




// std::string const Fichier("C:/Users/Edouard Balland/Desktop/graph_lib_v1/graph_lib_exemple_graph/image2.txt");
// std::ofstream Sommet(Fichier.c_str())



/* std::string image = "image.txt";
    std::ifstream fichier2(image);

    int indice1, indice2, indice3, indice5;
    double indice4;
    std::string nom;
    //Ouverture du fichier en lecture
    if(fichier2)
    {
        for(auto &elem : m_vertices)
        {
            fichier2>>indice1>>indice4>>indice2>>indice3>>nom>>indice5;



            //fichier2<<indice1<<" "<<indice4<<" "<< Pos_x <<" " <<  Pos_y << " "<< nom <<" "<<indice5<< std::endl;
        }

    }

    else
    {
        std::cout<<"Error"<<std::endl;
    }

fichier2.close();

std::ofstream fichier3 ("image.txt", std::ios::trunc);


for(auto &elem : m_vertices)
        {

            int Pos_x= elem.second.m_interface->m_top_box.get_frame().pos.x;
            int Pos_y=elem.second.m_interface->m_top_box.get_frame().pos.y;


            fichier3<<indice1<<" "<<indice4<<" "<< Pos_x <<" " <<  Pos_y << " "<< nom <<" "<<indice5<< std::endl;
        }


fichier3.close(); */
