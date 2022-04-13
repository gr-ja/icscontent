// Digraph.hpp
//
// ICS 46 Winter 2019
// Project #4: Rock and Roll Stops the Traffic
//
// This header file declares a class template called Digraph, which is
// intended to implement a generic directed graph. The implementation
// uses the adjacency lists technique, so each vertex stores a linked
// list of its outgoing edges.
//
// Along with the Digraph class template is a class DigraphException
// and a couple of utility structs that aren't generally useful outside
// of this header file.
//
// In general, directed graphs are all the same, except in the sense
// that they store different kinds of information about each vertex and
// about each edge; these two types are the type parameters to the
// Digraph class template.

#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <exception>
#include <functional>
#include <list>
#include <set>
#include <map>
#include <utility>
#include <algorithm>
#include <iostream>
#include <queue>
#include <limits>
# define INF 0x3f3f3f3f //infinity
using namespace std;


// DigraphExceptions are thrown from some of the member functions in the
// Digraph class template, so that exception is declared here, so it
// will be available to any code that includes this header file.

class DigraphException : public std::runtime_error
{
public:
    DigraphException(const std::string& reason);
};


inline DigraphException::DigraphException(const std::string& reason)
    : std::runtime_error{reason}
{
}



// A DigraphEdge lists a "from vertex" (the number of the vertex from which
// the edge points), a "to vertex" (the number of the vertex to which the
// edge points), and an EdgeInfo object.  Because different kinds of Digraphs
// store different kinds of edge information, DigraphEdge is a struct template.

template <typename EdgeInfo>
struct DigraphEdge
{
    int fromVertex;
    int toVertex;
    EdgeInfo einfo;
};



// A DigraphVertex includes two things: a VertexInfo object and a list of
// its outgoing edges.  Because different kinds of Digraphs store different
// kinds of vertex and edge information, DigraphVertex is a struct template.

template <typename VertexInfo, typename EdgeInfo>
struct DigraphVertex
{
    VertexInfo vinfo;
    std::list<DigraphEdge<EdgeInfo>> edges;
    DigraphVertex(){}
    DigraphVertex(VertexInfo input_Vinfo)
    :vinfo(input_Vinfo)
    {}
};



// Digraph is a class template that represents a directed graph implemented
// using adjacency lists.  It takes two type parameters:
//
// * VertexInfo, which specifies the kind of object stored for each vertex
// * EdgeInfo, which specifies the kind of object stored for each edge
//
// You'll need to implement the member functions declared here; each has a
// comment detailing how it is intended to work.
//
// Each vertex in a Digraph is identified uniquely by a "vertex number".
// Vertex numbers are not necessarily sequential and they are not necessarily
// zero- or one-based.

template <typename VertexInfo, typename EdgeInfo>
class Digraph
{
public:
    // The default constructor initializes a new, empty Digraph so that
    // contains no vertices and no edges.
    Digraph();

    // The copy constructor initializes a new Digraph to be a deep copy
    // of another one (i.e., any change to the copy will not affect the
    // original).
    Digraph(const Digraph& d);

    // The move constructor initializes a new Digraph from an expiring one.
    Digraph(Digraph&& d) noexcept;

    // The destructor deallocates any memory associated with the Digraph.
    ~Digraph() noexcept;

    // The assignment operator assigns the contents of the given Digraph
    // into "this" Digraph, with "this" Digraph becoming a separate, deep
    // copy of the contents of the given one (i.e., any change made to
    // "this" Digraph afterward will not affect the other).
    Digraph& operator=(const Digraph& d);

    // The move assignment operator assigns the contents of an expiring
    // Digraph into "this" Digraph.
    Digraph& operator=(Digraph&& d) noexcept;

    // vertices() returns a std::vector containing the vertex numbers of
    // every vertex in this Digraph.
    std::vector<int> vertices() const;

    // edges() returns a std::vector of std::pairs, in which each pair
    // contains the "from" and "to" vertex numbers of an edge in this
    // Digraph.  All edges are included in the std::vector.
    std::vector<std::pair<int, int>> edges() const;

    // This overload of edges() returns a std::vector of std::pairs, in
    // which each pair contains the "from" and "to" vertex numbers of an
    // edge in this Digraph.  Only edges outgoing from the given vertex
    // number are included in the std::vector.  If the given vertex does
    // not exist, a DigraphException is thrown instead.
    std::vector<std::pair<int, int>> edges(int vertex) const;

    // vertexInfo() returns the VertexInfo object belonging to the vertex
    // with the given vertex number.  If that vertex does not exist, a
    // DigraphException is thrown instead.
    VertexInfo vertexInfo(int vertex) const;

    // edgeInfo() returns the EdgeInfo object belonging to the edge
    // with the given "from" and "to" vertex numbers.  If either of those
    // vertices does not exist *or* if the edge does not exist, a
    // DigraphException is thrown instead.
    EdgeInfo edgeInfo(int fromVertex, int toVertex) const;

    // addVertex() adds a vertex to the Digraph with the given vertex
    // number and VertexInfo object.  If there is already a vertex in
    // the graph with the given vertex number, a DigraphException is
    // thrown instead.
    void addVertex(int vertex, const VertexInfo& vinfo);

    // addEdge() adds an edge to the Digraph pointing from the given
    // "from" vertex number to the given "to" vertex number, and
    // associates with the given EdgeInfo object with it.  If one
    // of the vertices does not exist *or* if the same edge is already
    // present in the graph, a DigraphException is thrown instead.
    void addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo);

    // removeVertex() removes the vertex (and all of its incoming
    // and outgoing edges) with the given vertex number from the
    // Digraph.  If the vertex does not exist already, a DigraphException
    // is thrown instead.
    void removeVertex(int vertex);

    // removeEdge() removes the edge pointing from the given "from"
    // vertex number to the given "to" vertex number from the Digraph.
    // If either of these vertices does not exist *or* if the edge
    // is not already present in the graph, a DigraphException is
    // thrown instead.
    void removeEdge(int fromVertex, int toVertex);

    // vertexCount() returns the number of vertices in the graph.
    int vertexCount() const noexcept;

    // edgeCount() returns the total number of edges in the graph,
    // counting edges outgoing from all vertices.
    int edgeCount() const noexcept;

    // This overload of edgeCount() returns the number of edges in
    // the graph that are outgoing from the given vertex number.
    // If the given vertex does not exist, a DigraphException is
    // thrown instead.
    int edgeCount(int vertex) const;

    // isStronglyConnected() returns true if the Digraph is strongly
    // connected (i.e., every vertex is reachable from every other),
    // false otherwise.
    bool isStronglyConnected() const;

    // findShortestPaths() takes a start vertex number and a function
    // that takes an EdgeInfo object and determines an edge weight.
    // It uses Dijkstra's Shortest Path Algorithm to determine the
    // shortest paths from the start vertex to every other vertex
    // in the graph.  The result is returned as a std::map<int, int>
    // where the keys are vertex numbers and the value associated
    // with each key k is the precedessor of that vertex chosen by
    // the algorithm.  For any vertex without a predecessor (e.g.,
    // a vertex that was never reached, or the start vertex itself),
    // the value is simply a copy of the key.
    std::map<int, int> findShortestPaths(
        int startVertex,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const;


private:


    // Add whatever member variables you think you need here.  One
    // possibility is a std::map where the keys are vertex numbers
    // and the values are DigraphVertex<VertexInfo, EdgeInfo> objects.

    map<int, DigraphVertex<VertexInfo, EdgeInfo>> vertexMap;


    // You can also feel free to add any additional member functions
    // you'd like (public or private), so long as you don't remove or
    // change the signatures of the ones that already exist.
};



// You'll need to implement the member functions below.  There's enough
// code in place to make them compile, but they'll all need to do the
// correct thing instead.

template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph()
{
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(const Digraph& d)
{
    vertexMap = d.vertexMap;
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(Digraph&& d) noexcept
{
    this->vertexMap = move(d.vertexMap);
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::~Digraph() noexcept
{
   this->vertexMap.clear();
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(const Digraph& d)
{
    this->vertexMap.clear();
    vertexMap = d.vertexMap;
    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(Digraph&& d) noexcept
{
    if(this != &d)
    {
        this->vertexMap = move(d.vertexMap);
    }

    return *this;
}

template <typename VertexInfo, typename EdgeInfo>
std::vector<int> Digraph<VertexInfo, EdgeInfo>::vertices() const
{

    vector<int> vertexNumber;
    for (typename map<int, DigraphVertex<VertexInfo, EdgeInfo>>::const_iterator it=vertexMap.begin();it!=vertexMap.end(); ++it)
    {
        vertexNumber.push_back(it->first);
    }

    return vertexNumber;


}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges() const
{

    vector<pair<int,int>> result;
    for(typename map<int,DigraphVertex<VertexInfo,EdgeInfo>>::const_iterator iter=vertexMap.begin();iter!=vertexMap.end();iter++)
    {
        for(typename list<DigraphEdge<EdgeInfo>>::const_iterator it = iter->second.edges.begin();it!=iter->second.edges.end();it++)
            result.push_back(make_pair(it->fromVertex,it->toVertex));
    }


    return result;
}

template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges(int vertex) const
{
    if(vertexMap.count(vertex)==0)
        throw DigraphException("vertex does not exist");
    vector<std::pair<int, int>> result;
    for(typename list<DigraphEdge<EdgeInfo>>::const_iterator it = vertexMap.at(vertex).edges.begin();it!=vertexMap.at(vertex).edges.end();it++)
        result.push_back(make_pair(it->fromVertex,it->toVertex));

    return result;
}


template <typename VertexInfo, typename EdgeInfo>
VertexInfo Digraph<VertexInfo, EdgeInfo>::vertexInfo(int vertex) const
{
    if(vertexMap.count(vertex)==0)
        throw DigraphException("vertexMap is empty, and vertex does not exist");
    vector<std::pair<int, int>> result;
    for(typename map<int,DigraphVertex<VertexInfo,EdgeInfo>>::const_iterator iter=vertexMap.begin();iter!=vertexMap.end();iter++)
    {
        if(iter->first == vertex)
            return iter->second.vinfo;
    }

    throw DigraphException("vertex does not exist");
}


template <typename VertexInfo, typename EdgeInfo>
EdgeInfo Digraph<VertexInfo, EdgeInfo>::edgeInfo(int fromVertex, int toVertex) const
{
    vector<std::pair<int, int>> result;
    for(typename map<int,DigraphVertex<VertexInfo,EdgeInfo>>::const_iterator iter=vertexMap.begin();iter!=vertexMap.end();iter++)
    {
        for(typename list<DigraphEdge<EdgeInfo>>::const_iterator it = iter->second.edges.begin();it!=iter->second.edges.end();it++) {
            if (it->fromVertex == fromVertex && it->toVertex == toVertex)
                return it->einfo;
        }
    }

    throw DigraphException("The edge does not exist....");
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addVertex(int vertex, const VertexInfo& vinfo)
{
    for(typename map<int,DigraphVertex<VertexInfo,EdgeInfo>>::const_iterator iter=vertexMap.begin();iter!=vertexMap.end();iter++)
    {
        if(iter->first == vertex)
            throw DigraphException("The vertex is exist in the map");
    }

    DigraphVertex<VertexInfo,EdgeInfo> DV = DigraphVertex<VertexInfo,EdgeInfo>(vinfo);
    vertexMap.insert(make_pair(vertex,DV));


}

template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo) {
    if (vertexMap.count(fromVertex) == 0 || vertexMap.count(toVertex) == 0)
        throw DigraphException("vertex number does not exist");

    vector<std::pair<int,int>> edgesVector = edges(fromVertex);
    pair<int,int> check = pair(fromVertex,toVertex);
    if(find(edgesVector.begin(),edgesVector.end(),check)!=edgesVector.end())
        throw DigraphException("Invalid edge");


    vertexMap.at(fromVertex).edges.push_back(DigraphEdge<EdgeInfo>{fromVertex, toVertex, einfo});
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeVertex(int vertex)
{
    if(vertexMap.count(vertex)==0)
        throw DigraphException("vertex does not exist");

    for(typename map<int,DigraphVertex<VertexInfo,EdgeInfo>>::iterator iter=vertexMap.begin();iter!=vertexMap.end();iter++)
    {
        typename std::list<DigraphEdge<EdgeInfo>>::iterator it = iter->second.edges.begin();
        while(it!=iter->second.edges.end())
        {
            if(it->toVertex == vertex)
                iter->second.edges.erase(it);
            it++;
        }
    }
    vertexMap.erase(vertex);

}



template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeEdge(int fromVertex, int toVertex)
{
    if (vertexMap.count(fromVertex) == 0 || vertexMap.count(toVertex) == 0)
        throw DigraphException("vertex number does not exist");

    int trigger  = 1;
    for(typename map<int,DigraphVertex<VertexInfo,EdgeInfo>>::iterator iter=vertexMap.begin();iter!=vertexMap.end();iter++)
    {
        typename std::list<DigraphEdge<EdgeInfo>>::iterator it = iter->second.edges.begin();
        while(it!=iter->second.edges.end())
        {
            if(it->toVertex == toVertex && it->fromVertex == fromVertex)
            {
                it = iter->second.edges.erase(it);
                trigger = 0;
            } else
            {
                it++;
            }
        }

    if (trigger ==1)
        throw DigraphException("vertex number does not exist");
}
}



template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::vertexCount() const noexcept
{
    return vertexMap.size();
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount() const noexcept
{
    int counter = 0;

    for(typename map<int,DigraphVertex<VertexInfo,EdgeInfo>>::const_iterator iter=vertexMap.begin();iter!=vertexMap.end();iter++)
    {
        counter = counter + iter->second.edges.size();
    }
    return  counter;
}
template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount(int vertex) const
{
   if(vertexMap.count(vertex) == 0)
       throw DigraphException("vertex number does not exist");
    return edges(vertex).size();

}


template <typename VertexInfo, typename EdgeInfo>
bool Digraph<VertexInfo, EdgeInfo>::isStronglyConnected() const
{
   if ( edges().size()< vertexMap.size())
        return false;
   else
   {
       for(typename map<int,DigraphVertex<VertexInfo,EdgeInfo>>::const_iterator iter=vertexMap.begin();iter!=vertexMap.end();iter++)
       {
           queue<int> vertexQueue;
           set<int>   vertexSet;
           vertexQueue.push(iter->first);
           while(!vertexQueue.empty())
           {
               int s = vertexQueue.front();
               vertexQueue.pop();
               vertexSet.insert(s);
               typename std::list<DigraphEdge<EdgeInfo>>::const_iterator it = vertexMap.at(s).edges.begin();
               while(it!=vertexMap.at(s).edges.end())
               {
                   if(vertexSet.count(it->toVertex)==0)
                   {
                       vertexSet.insert(it->toVertex);
                       vertexQueue.push(it->toVertex);
                   } else{
                       it++;
                   }
               }
           }
           if (vertexSet.size() == vertexMap.size())
               return true;
       }
       return false;
   }
}

struct Node
{
    int vertexNumber;
    double edgeWeight;
    Node(){}
    Node(int vn, double ew)
    :vertexNumber(vn), edgeWeight(ew)
    {};
};

struct cmp
{
    bool operator ()(const Node & a,const Node & b)
    {
       return a.edgeWeight > b.edgeWeight;
    }
};

template <typename VertexInfo, typename EdgeInfo>
std::map<int, int> Digraph<VertexInfo, EdgeInfo>::findShortestPaths(
    int startVertex,
    std::function<double(const EdgeInfo&)> edgeWeightFunc) const

{
    priority_queue<Node , std::vector<Node>, cmp> pq;
    double d =numeric_limits<double>::max() ;
    map<int, int> shortestPath;

    map<int, pair<double,bool>> dist;

    for(typename map<int,DigraphVertex<VertexInfo,EdgeInfo>>::const_iterator iter=vertexMap.begin();iter!=vertexMap.end();iter++)
    {
        dist[iter->first].first = d;
        dist[iter->first].second = false;
    }


    pq.push(Node(startVertex,0));
    shortestPath[startVertex] = startVertex;
    dist[startVertex].first = 0;

    while (!pq.empty())
    {
        Node currentSmallest = pq.top();
        pq.pop();
        if (dist[currentSmallest.vertexNumber].second == false)
        {
            dist[currentSmallest.vertexNumber].second = true;
            for(typename list<DigraphEdge<EdgeInfo>>::const_iterator it = vertexMap.at(currentSmallest.vertexNumber).edges.begin();it!=vertexMap.at(currentSmallest.vertexNumber).edges.end();it++)
            {
                int v = it->toVertex;
                double edgeWeight = edgeWeightFunc(it->einfo);


                if(dist[v].first > dist[currentSmallest.vertexNumber].first + edgeWeight)
                {

                    dist[v].first = dist[currentSmallest.vertexNumber].first + edgeWeight;
                    shortestPath[v]= currentSmallest.vertexNumber;
                    pq.push(Node(v,dist[v].first));


                }



            }

        }

    }

    return shortestPath;


}



#endif // DIGRAPH_HPP