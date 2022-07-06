#include <iterator>
#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <map>
#include <vector>
#include <queue>
#include <list>
#include <set>
#include <math.h>
#include "assert.h"
#include <algorithm>
#include "string.h"
#include <stdio.h>
#include <numeric> 





#define INSERTED  1
#define UNINSERTED 0

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <stdio.h>
#include <stdlib.h>

#define MAX_UINT_CONSTANT     0xffffffff 
#define MAX_UINT_CONSTANT16   0xffff
#define MAX_UINT_CONSTANT8    0xff
#define GreaterArrow          0x1 
#define LessArrow             0x2       
#define SIGNED     0x80000000
#define MAX_INT64_CONSTANT 0xffffffffffffffff
#define int64 long long
#define MAX_Stack_Size 0xffff

#define _(String) gettext(String)

#undef MAX
#undef MIN
#define MAX(a, b) (((a) >= (b)) ? (a) : (b))
#define MIN(a, b) (((a) <= (b)) ? (a) : (b))
#define INS   (1<<8)
#define DEL   (2<<8)
#define SUB1  (3<<8)
#define SUB2  (4<<8)




typedef struct Edge {
    int16_t elabel;
    unsigned int EdgeId;
    unsigned int VertexId;
} Edge;

typedef struct Vertex {
     int16_t vlabel;
     std::vector<Edge> edge_list; 
} Vertex;

typedef struct Graph_ {
    std::vector<Vertex> vertex_List; 
    unsigned int EdgeSize;
}Graph_;

 typedef struct PDFS {
    unsigned int ID;    // The ID of the original input graphs 
    unsigned int StartID;  // The location  of the start vertex in the graphs 
    unsigned int EndID; // The location  of the end vertex in the graphs 
    unsigned int parentID; // The location in the parent DFS's project list. 
}PDFS;

typedef struct DFS {
    int32_t elabel;// Edge label in the graph 
    int16_t vlabelTo;
    int16_t vlabelFrom;  
    unsigned int  to;
    unsigned int from;
    unsigned int rootID;
    unsigned int support; 
    bool isTraveled; 
    std::vector <PDFS> Projected;
}DFS;

 typedef  struct rootComposeID{
     unsigned  int ID;
     unsigned  int subID;
 }rootComposeID;






extern bool  ComparerootID(rootComposeID A,rootComposeID B); 
extern bool  CompareVector(const std::vector<unsigned int> &a, const std::vector<unsigned int> &b);




 typedef struct PartialOrderKey{
   unsigned int to;
   unsigned int from;
   int32_t elabel;
   int16_t vlabelTo;
   int16_t vlabelFrom; 

 }PartialOrderKey;


 typedef struct PDFSMin {
    uint16_t ID;    // The edge ID  
    uint32_t parentID; // The location in the parent edge ID 
}PDFSMin;


 typedef struct PartialOrderKeyMinimum{
   uint16_t to;
   uint16_t from;
   uint16_t ancestor;
   uint16_t rank;
   uint32_t begin;
   uint32_t end;
 }PartialOrderKeyMinimum;








 typedef struct PartialOrderKeyThin{
   int32_t elabel;
   int16_t vlabelTo;
   int16_t vlabelFrom; 
 }PartialOrderKeyThin;

struct PartialOrderKeyThinCompare
{
    bool operator()( const PartialOrderKeyThin &a, const PartialOrderKeyThin &b) const
   {
         if( a.vlabelFrom == b.vlabelFrom )
         {
             if(  a.elabel == b.elabel )
             {
                    return (a.vlabelTo > b.vlabelTo);

             }else
             {
                    return (a.elabel > b.elabel);
             }
         }
        else
         { 
            return (a.vlabelFrom > b.vlabelFrom); 
         }
      return false;
   }

};





struct PartialOrderCompare
{

  bool operator()( const PartialOrderKey &a, const PartialOrderKey &b) const
  {


     if( (a.to==b.to) && (a.from==b.from) ) 
     {

         if( a.vlabelFrom == b.vlabelFrom )
         {
             if(  a.elabel == b.elabel )
             {
                    return (a.vlabelTo > b.vlabelTo);

             }else
             {
                    return (a.elabel > b.elabel);
             }

         }
        else
         { 
           return (a.vlabelFrom > b.vlabelFrom); 
         }
     }
     if((a.from < a.to )&&(b.from < b.to ))
     {
        if(a.to == b.to)
        {
           return (a.from > b.from);   
        }else
        {
            return (a.to < b.to); 
        }
     }
     if((a.from > a.to )&&(b.from > b.to ))
     {
         if(a.from==b.from)
         {
            return (a.to < b.to);
         }else
         {
           return (a.from < b.from);
         }
     }
     if((a.from < a.to )&&(b.from > b.to ))
     {
         return(a.to <= b.from);
     }
     if((a.from > a.to )&&(b.from < b.to ))
     {
         return(a.from < b.to);
     }


      return   false;
   }
};// Linear order 

struct compare
{
    bool operator()(const std::vector<unsigned int> &a, const std::vector<unsigned int> &b) const
    {

            if(a.size()<b.size())
            {
                return false;
            }
            if(a.size()>b.size())
            {
                return true;
            }
            for(int i=0;i<a.size();i++)
            {
                    if (a[i] > b[i])
                    {
                        return  true;
                    }
                    if(a[i] < b[i])
                    {
                        return  false;
                    }
            }
               return  false;
    }
};


#define  MAX_COMPARESIZE  4096
#ifndef  COMPARECONTEXT
#define  COMPARECONTEXT  0
extern unsigned int CompareContextA[MAX_COMPARESIZE];
extern unsigned int CompareContextB[MAX_COMPARESIZE];
extern unsigned int DegreeContextA[MAX_COMPARESIZE];
extern unsigned int DegreeContextB[MAX_COMPARESIZE];
#endif







struct GraphCompare
{
  

    bool operator ()( const Graph_ &g1,const Graph_ &g2 ) const
   {
     

       unsigned int CompareContext1_size = g1.EdgeSize;
       unsigned int CompareContext2_size = g2.EdgeSize;

      if(CompareContext1_size != CompareContext2_size)
      {
         return (CompareContext1_size > CompareContext2_size); 
      }
      if((CompareContext1_size==0)&&(CompareContext2_size==0))
      {
        return false; 
      }


     for(int i=0;i<g1.vertex_List.size();i++)
     {
        DegreeContextA[i] = g1.vertex_List[i].edge_list.size();
     } 


    for(int i=0;i<g2.vertex_List.size();i++)
     {
        DegreeContextB[i] = g2.vertex_List[i].edge_list.size();
     } 

    for(int i=0;i<g1.vertex_List.size();i++)
    {
           for( int j =0;j<g1.vertex_List[i].edge_list.size();j++)
            {
                 DegreeContextA[g1.vertex_List[i].edge_list[j].VertexId]++;
            }
    }


 
    for(int i=0;i<g2.vertex_List.size();i++)
    {
           for( int j =0;j<g2.vertex_List[i].edge_list.size();j++)
            {
                DegreeContextB[g2.vertex_List[i].edge_list[j].VertexId]++;
            }
    }








     for(int i=0;i<g1.vertex_List.size();i++)
        {
           for( int j =0;j<g1.vertex_List[i].edge_list.size();j++)
            {

                unsigned int labelA = g1.vertex_List[i].vlabel;
                unsigned int labelB = g1.vertex_List[g1.vertex_List[i].edge_list[j].VertexId].vlabel;
                unsigned EdgeId = g1.vertex_List[i].edge_list[j].EdgeId;
                unsigned int degreeA = DegreeContextA[i]/2;
                unsigned int degreeB = DegreeContextA[g1.vertex_List[i].edge_list[j].VertexId]/2;



                CompareContextA[EdgeId]  = std::min(labelA,labelB)+64*std::min(degreeA,degreeB);
                CompareContextA[EdgeId]  = CompareContextA[EdgeId] << 8;
                CompareContextA[EdgeId]  = CompareContextA[EdgeId]+std::max(labelA,labelB)+64*std::max(degreeA,degreeB);
                CompareContextA[EdgeId]  = CompareContextA[EdgeId] << 8;
                CompareContextA[EdgeId] |= g1.vertex_List[i].edge_list[j].elabel;
            }
        }

       for(int i=0;i<g2.vertex_List.size();i++)
        {
            for( int j =0;j<g2.vertex_List[i].edge_list.size();j++)
            {

                unsigned int labelA = g2.vertex_List[i].vlabel;
                unsigned int labelB = g2.vertex_List[g2.vertex_List[i].edge_list[j].VertexId].vlabel;
                unsigned EdgeId = g2.vertex_List[i].edge_list[j].EdgeId;
                unsigned int degreeA = DegreeContextB[i]/2;
                unsigned int degreeB = DegreeContextB[g2.vertex_List[i].edge_list[j].VertexId]/2;

                
                CompareContextB[EdgeId]  = std::min(labelA,labelB)+64*std::min(degreeA,degreeB);
                CompareContextB[EdgeId]  = CompareContextB[EdgeId] << 8;
                CompareContextB[EdgeId]  = CompareContextB[EdgeId]+std::max(labelA,labelB)+64*std::max(degreeA,degreeB);
                CompareContextB[EdgeId]  = CompareContextB[EdgeId] << 8;
                CompareContextB[EdgeId] |= g2.vertex_List[i].edge_list[j].elabel;
            }
        }


      std::sort(&CompareContextA[0],&CompareContextA[CompareContext1_size]);
      std::sort(&CompareContextB[0],&CompareContextB[CompareContext2_size]);


       {
            int i,j;

            for( i=0,j=0;i<CompareContext1_size&&j<CompareContext2_size;)
            {
                        
                 {
                            if(CompareContextA[i]==CompareContextB[j])
                            {
                                i++;
                                j++;
                                continue;
                            }
                            if(CompareContextA[i]>CompareContextB[j])
                            {
                                return true;
                            }
                            if(CompareContextA[i]<CompareContextB[j])
                            {
                               return false;  
                            }
                }
          }

             return false; 
       } 

   }
};


typedef struct ExtensionCount
{
    unsigned  ID;
    unsigned  Count;

}ExtensionCount;




typedef std::map < Graph_ ,std::vector < unsigned int > ,GraphCompare >  MPINDEX;

typedef std::map < PartialOrderKey,std::vector < PDFS >,PartialOrderCompare > Projected_map; 
typedef std::map < PartialOrderKey,uint16_t,PartialOrderCompare > Projected_count_map; 
typedef std::map < std::vector<unsigned int>,uint32_t> SubSetMapping; 





typedef struct DFSPath_T: public std::vector <DFS> {
public:
    void push (unsigned int Population,uint32_t elabel,uint16_t vlabelFrom,uint16_t vlabelTo,std::vector <PDFS>&Projected,unsigned int rootID,unsigned int from,unsigned int to)
    {
        resize (size() + 1);
        DFS &d = (*this)[size()-1];
        d.elabel = elabel;
        d.vlabelFrom = vlabelFrom;
        d.vlabelTo   = vlabelTo;
        d.rootID = rootID;
        d.from =  from; 
        d.to = to;
        d.Projected = Projected;
        d.isTraveled = false; 
        d.support = Population;
    }
    void pop () { resize (size()-1); }
}DFSPath_T;//The type of depth first search




class ComStruct{
private:

    Projected_map new_projected; 
    Projected_count_map new_projected_count;
    DFSPath_T DFSpath;
    std::set < std::vector<unsigned int>,compare > filterset;
    std::set < PartialOrderKeyThin,PartialOrderKeyThinCompare > EdgeFilterset;
    std::vector< std::vector< uint8_t > > IsGFLE; //general forward leaf edge 
    std::vector<Graph_> GraphPair;
    unsigned int CompareContext1[4096];
    unsigned int CompareContext2[4096];
    PartialOrderKey  minimumGraphContext[4096];
    PartialOrderKeyMinimum  minimumGraphStack[4096];
    unsigned int minimumGraphStacksize;
    PDFSMin minimumGraphStackList[MAX_Stack_Size];
    bool minimumGraphTraveled[4096];
    uint16_t node_mapping[4096]; 
    ExtensionCount   FullextensionSupportContext[4096*8];
    unsigned int     FullextensionOccurrenceContext[4096*8];
    bool FullextensionIsTransitiveContext[4096*8];
  

public:

    std::vector<Graph_>Sample;
    void RemoveDuplicates();
    std::vector<Graph_>FrequentGraphs;
    std::vector< unsigned int > SubSet; 
    std::vector< uint32_t>FrequentGraphsIDArray;
    SubSetMapping  FrequentGraphsIDArrayMapping; 
    std::vector< std::vector<unsigned int> >FrequentGraphsOccurrenceArray; 
    std::vector< bool > OccurringTransitive;
    std::vector< uint32_t >  supportFingerprintPath; 
    std::vector< PartialOrderKey > minimumCodeTrace; 
    bool  Is_minimum( unsigned int ID,unsigned int to,unsigned int from,uint16_t vlabelFrom,uint16_t vlabelTo,uint32_t elabel,bool isTraced);
    void  supportFingerprint(uint32_t &ArrayID,std::vector < PDFS > & Projected_);
    bool  Verbose;
    bool  Trim = false;
    unsigned int minsup;
    void VectorOrderDeterminated(Graph_&g);
    void FrequentGraphMining();
    bool IsConteined(Graph_&superGraph,Graph_&subGraph);
    int  GraphReadRemoveDuplicatedEdge(FILE * InputName);
    int  GraphRead(FILE * InputName);
    bool OutPutGraphNode(Graph_ &g1,FILE * f1,unsigned int Index);
    bool isTrace; 
    bool isSelected=false;
    std::vector< std::vector<unsigned int> > TraceG1; 
    std::vector< std::vector<unsigned int> > TraceG2;
    unsigned  CountG1;
    unsigned  CountG2;
    unsigned int maxStackSize_ = 4096; 
    
    std::vector< std::vector< std::vector<unsigned int> > > TraceArray; 
    std::vector< std::vector<unsigned int> > TraceSingle;
    std::vector<unsigned int> MatchedGraphsIDCountArray;
    std::vector<unsigned int> MatchedGraphsIDArray;
    unsigned MatchedSingleGraphCount; 
    bool isLeaf(std::vector < PDFS > & Projected_,int32_t EdgeLabel);

    bool graphMatch(Graph_ &g1,Graph_ &g2);
    bool graphBulkMatch(Graph_ &g1);
    bool OutPutGraph(Graph_ &g1,FILE * f1);


};


class UniqueGraphSet{

    MPINDEX GraphIndex;
    ComStruct Match;
    public:
    std::vector<Graph_> GraphSet;
    UniqueGraphSet();
    bool  MPinsert(Graph_& g);
    unsigned int  MPfind(Graph_& g);
    
};























