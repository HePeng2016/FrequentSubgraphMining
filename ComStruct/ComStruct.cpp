#include "ComStruct.h"

bool  ComparerootID(rootComposeID A,rootComposeID B)
{
    if(A.ID == B.ID)
    {
        return A.subID < B.subID;
    }else{
        return A.ID  < B.ID;
    }
}



unsigned int CompareContextA[MAX_COMPARESIZE];
unsigned int CompareContextB[MAX_COMPARESIZE];
unsigned int DegreeContextA[MAX_COMPARESIZE];
unsigned int DegreeContextB[MAX_COMPARESIZE];




bool CompareVector(const std::vector<unsigned int> &a, const std::vector<unsigned int> &b)
{
      if(a.size()==b.size())
      {
            for(int i=0;i<a.size();i++)
            {
                    if (a[i]!=b[i])
                    {
                        return  a[i]<b[i];
                    }
            }
             return false;
      }else
      {
         return a.size()>b.size();
      }
}


bool IsEqual(const std::vector<unsigned int> &a, const std::vector<unsigned int> &b)
{
      if(a.size()==b.size())
      {
            for(int i=0;i<a.size();i++)
            {
                    if (a[i]!=b[i])
                    {
                        return  false;
                    }
            }
                        return true;
      }else
      {
                        return false;
      }
}







void  Support(unsigned int &size,std::vector < PDFS > & Projected_) {
   unsigned int oldID = 0xffffffff;
   size =0;
   for (std::vector<PDFS>::iterator cur = Projected_.begin(); cur != Projected_.end(); ++cur) {
    if (oldID != cur->ID) {
        size=size+1;}
        oldID=cur->ID;}
       }



void  OccurringMapping(unsigned int &size,std::vector < PDFS > & Projected_) {
   unsigned int oldID = 0xffffffff;
   size =0;
   for (std::vector<PDFS>::iterator cur = Projected_.begin(); cur != Projected_.end(); ++cur) {
    if (oldID != cur->parentID) {
        size=size+1;}
        oldID=cur->parentID;}
       }
bool ComStruct::isLeaf(std::vector < PDFS > & Projected_,int32_t EdgeLabel)
{
      for (std::vector<PDFS>::iterator cur = Projected_.begin(); cur != Projected_.end(); ++cur) {

          if( Sample[cur->ID].vertex_List[cur->EndID].edge_list.size()==1 )
          {
                return true;
          }else
          {  
               for(int i=0;i<Sample[cur->ID].vertex_List[cur->EndID].edge_list.size();i++)
               {
                  if((Sample[cur->ID].vertex_List[cur->EndID].edge_list[i].VertexId==cur->StartID)&&(Sample[cur->ID].vertex_List[cur->EndID].edge_list[i].elabel==EdgeLabel))
                  {
                        unsigned int EdgeID = Sample[cur->ID].vertex_List[cur->EndID].edge_list[i].EdgeId;

                        if(( (cur->EndID > cur->StartID)&&((IsGFLE[cur->ID][EdgeID]&GreaterArrow)!=0) )||( (cur->EndID < cur->StartID)&&(IsGFLE[cur->ID][EdgeID]&LessArrow)!=0))
                        {
                            return true;
                        }

                  }
               }

              //General forward leaf edge; 
          }
       }
    return false;
}



void  ComStruct::supportFingerprint(uint32_t & ArrayID,std::vector < PDFS > & Projected_)
{ 
        SubSetMapping::iterator it;
        unsigned int oldID = 0xffffffff;
        std::vector < unsigned int > FrequentGraphsIDEntry;

        for (std::vector<PDFS>::iterator cur = Projected_.begin(); cur != Projected_.end(); ++cur) {

               if (oldID != cur->ID) {
                    FrequentGraphsIDEntry.resize(FrequentGraphsIDEntry.size()+1);
                    FrequentGraphsIDEntry[FrequentGraphsIDEntry.size()-1]=cur->ID;
                    }
                    oldID=cur->ID;
                 }

        it = FrequentGraphsIDArrayMapping.find(FrequentGraphsIDEntry);

        if( it!=FrequentGraphsIDArrayMapping.end())
        {
            ArrayID = it->second;
        }else
        {
            ArrayID = FrequentGraphsIDArrayMapping.size()+1;
            FrequentGraphsIDArrayMapping.insert(make_pair(FrequentGraphsIDEntry,ArrayID));
        }
  }

int  ComStruct::GraphReadRemoveDuplicatedEdge(FILE * InputName)
{

    std::set < PartialOrderKey,PartialOrderCompare > UniqueEdge; 

    unsigned int oldID=MAX_UINT_CONSTANT; 
    unsigned int ID=-1; 
    unsigned int Size_; 
    char buffer[1024];




    Sample.resize(0);


    while((!feof(InputName)))
    {

        if(fscanf(InputName,"%u",&Size_)==EOF)
        {
             return 0;
        };
        fscanf(InputName,"%[\b|\t|,|\n]*",buffer);
        Sample.resize(Sample.size()+1);
        ID = ID+1;
    
        Sample[ID].vertex_List.resize(Size_);

        for(int i=0;i<Size_;i++)
        {
           fscanf(InputName,"%[\b|\t|,]*",buffer);
           if(fscanf(InputName,"%d",&(Sample[ID].vertex_List[i].vlabel))==EOF) return 0;  
           fscanf(InputName,"%[\b|\t|,|\n]*",buffer);
        }

           fscanf(InputName,"%[\b|\t|,|\n]*",buffer);
           if(fscanf(InputName,"%u",&Size_)==EOF) return 0;
           fscanf(InputName,"%[\b|\t|,|\n]*",buffer);

           UniqueEdge.clear();


        for(int i=0;i<Size_;i++)
        {
            unsigned int from;
            unsigned int to;
            unsigned int edgeLabel; 
            PartialOrderKey key;

           fscanf(InputName,"%[\b|\t|,]*",buffer);
           if(fscanf(InputName,"%u",&from)==EOF) return 0; 
           fscanf(InputName,"%[\b|\t|,]*",buffer); 
           if(fscanf(InputName,"%u",&to)==EOF) return 0; 
           fscanf(InputName,"%[\b|\t|,]*",buffer);
           if(fscanf(InputName,"%d",&edgeLabel)==EOF) return 0; 

           if(from<to)
           {
              key.to         =  to;
              key.from       =  from; 
              key.elabel     =  edgeLabel;
              key.vlabelTo   =  Sample[ID].vertex_List[to].vlabel;
              key.vlabelFrom =  Sample[ID].vertex_List[from].vlabel;


           }else
           { 
               key.to           =  from;
               key.from         =  to; 
               key.elabel       =  edgeLabel;
               key.vlabelFrom   =  Sample[ID].vertex_List[to].vlabel;
               key.vlabelTo     =  Sample[ID].vertex_List[from].vlabel;
            }


               if(UniqueEdge.find(key)!=UniqueEdge.end())
              {
                 fscanf(InputName,"%[\b|\t|,|\n]*",buffer);
                 fscanf(InputName,"%[\b|\t|,]*",buffer); 
                 continue;
              }else
              {
                 UniqueEdge.insert(key);
              }




           Sample[ID].vertex_List[from].edge_list.resize(Sample[ID].vertex_List[from].edge_list.size()+1); 
           Sample[ID].vertex_List[from].edge_list[Sample[ID].vertex_List[from].edge_list.size()-1].elabel = edgeLabel;
           Sample[ID].vertex_List[from].edge_list[Sample[ID].vertex_List[from].edge_list.size()-1].VertexId = to; 
           Sample[ID].vertex_List[from].edge_list[Sample[ID].vertex_List[from].edge_list.size()-1].EdgeId = i;


           Sample[ID].vertex_List[to].edge_list.resize(Sample[ID].vertex_List[to].edge_list.size()+1); 
           Sample[ID].vertex_List[to].edge_list[Sample[ID].vertex_List[to].edge_list.size()-1].elabel = edgeLabel;
           Sample[ID].vertex_List[to].edge_list[Sample[ID].vertex_List[to].edge_list.size()-1].VertexId = from; 
           Sample[ID].vertex_List[to].edge_list[Sample[ID].vertex_List[to].edge_list.size()-1].EdgeId = i;
           //  undirected Graph (Double Linker)  

            fscanf(InputName,"%[\b|\t|,|\n]*",buffer);
            fscanf(InputName,"%[\b|\t|,]*",buffer);
          
        }
             Sample[ID].EdgeSize = Size_;

             fscanf(InputName,"%[\b|\t|,]*",buffer);
             fscanf(InputName,"%[\b|\t|,|\n]*",buffer);

     }

      return 1;

} 



int  ComStruct::GraphRead(FILE * InputName)
{

    unsigned int oldID=MAX_UINT_CONSTANT; 
    unsigned int ID=-1; 
    unsigned int Size_; 
    char buffer[1024];




    Sample.resize(0);
    IsGFLE.resize(0);


    while((!feof(InputName)))
    {

        if(fscanf(InputName,"%u",&Size_)==EOF)
        {
             return 0;
        };
        fscanf(InputName,"%[\b|\t|,|\n]*",buffer);
        Sample.resize(Sample.size()+1);
        ID = ID+1;
    
        Sample[ID].vertex_List.resize(Size_);

        for(int i=0;i<Size_;i++)
        {
           fscanf(InputName,"%[\b|\t|,]*",buffer);
           if(fscanf(InputName,"%d",&(Sample[ID].vertex_List[i].vlabel))==EOF) return 0;  
           fscanf(InputName,"%[\b|\t|,|\n]*",buffer);
        }

           fscanf(InputName,"%[\b|\t|,|\n]*",buffer);
           if(fscanf(InputName,"%u",&Size_)==EOF) return 0;
           fscanf(InputName,"%[\b|\t|,|\n]*",buffer);

        for(int i=0;i<Size_;i++)
        {
            unsigned int from;
            unsigned int to;
            unsigned int edgeLabel; 

           fscanf(InputName,"%[\b|\t|,]*",buffer);
           if(fscanf(InputName,"%u",&from)==EOF) return 0; 
           fscanf(InputName,"%[\b|\t|,]*",buffer); 
           if(fscanf(InputName,"%u",&to)==EOF) return 0; 
           fscanf(InputName,"%[\b|\t|,]*",buffer);
           if(fscanf(InputName,"%d",&edgeLabel)==EOF) return 0; 

           Sample[ID].vertex_List[from].edge_list.resize(Sample[ID].vertex_List[from].edge_list.size()+1); 
           Sample[ID].vertex_List[from].edge_list[Sample[ID].vertex_List[from].edge_list.size()-1].elabel = edgeLabel;
           Sample[ID].vertex_List[from].edge_list[Sample[ID].vertex_List[from].edge_list.size()-1].VertexId = to; 
           Sample[ID].vertex_List[from].edge_list[Sample[ID].vertex_List[from].edge_list.size()-1].EdgeId = i;


           Sample[ID].vertex_List[to].edge_list.resize(Sample[ID].vertex_List[to].edge_list.size()+1); 
           Sample[ID].vertex_List[to].edge_list[Sample[ID].vertex_List[to].edge_list.size()-1].elabel = edgeLabel;
           Sample[ID].vertex_List[to].edge_list[Sample[ID].vertex_List[to].edge_list.size()-1].VertexId = from; 
           Sample[ID].vertex_List[to].edge_list[Sample[ID].vertex_List[to].edge_list.size()-1].EdgeId = i;
           //  undirected Graph (Double Linker)  

            fscanf(InputName,"%[\b|\t|,|\n]*",buffer);
            fscanf(InputName,"%[\b|\t|,]*",buffer);
          
        }
             Sample[ID].EdgeSize = Size_;

             fscanf(InputName,"%[\b|\t|,]*",buffer);
             fscanf(InputName,"%[\b|\t|,|\n]*",buffer);

     }

     return 1;

}



void ComStruct::FrequentGraphMining()
{

       filterset.clear();
       FrequentGraphs.resize(0);
       FrequentGraphsIDArrayMapping.clear();
       FrequentGraphsIDArray.resize(0);
       EdgeFilterset.clear();
       new_projected.clear();
       new_projected_count.clear();
                        

       DFSpath.clear();
       OccurringTransitive.resize(0);
       //supportFingerprintPath.resize(0);

       if(IsGFLE.size() == 0)
       { 
          IsGFLE.resize(Sample.size());
          for(int i=0;i<IsGFLE.size();i++)
          {
              IsGFLE[i].resize(0);
          }
       }










       if(isSelected==true)
       {

                      for (int i = 0;i<SubSet.size();i++) 
                      { 




                          if(IsGFLE[SubSet[i]].size()==0)
                          {
                             IsGFLE[SubSet[i]].resize(Sample[SubSet[i]].EdgeSize);

                             for(int i1=0;i1<IsGFLE[SubSet[i]].size();i1++)
                             {
                                  IsGFLE[SubSet[i]][i1] = 0;
                             }


                             unsigned int  AddedleafN = 0;

                             for(int j=0;j<Sample[SubSet[i]].vertex_List.size();j++)
                             {

                                if(Sample[SubSet[i]].vertex_List[j].edge_list.size()==1)
                                {
                                    unsigned int To_   =  j;
                                    unsigned int From_ =  Sample[SubSet[i]].vertex_List[j].edge_list[0].VertexId;  


                                    if((To_ > From_))
                                    {
                                        IsGFLE[SubSet[i]][Sample[SubSet[i]].vertex_List[j].edge_list[0].EdgeId] = GreaterArrow;
                                    }else
                                    {
                                        IsGFLE[SubSet[i]][Sample[SubSet[i]].vertex_List[j].edge_list[0].EdgeId] = LessArrow;
                                    }

                                    AddedleafN = AddedleafN+1;
                                }

                             }

                             while(AddedleafN!=0)
                             {
                                  
                                  AddedleafN = 0;


                                 for(int j=0;j<Sample[SubSet[i]].vertex_List.size();j++)
                                 {

                                    unsigned int ancestorEdgeN   = 0; 
                                    unsigned int ancestorEdgeID  = MAX_UINT_CONSTANT;
                                    unsigned int ancestorVertexID = MAX_UINT_CONSTANT;
                                    unsigned int To_  =  MAX_UINT_CONSTANT;
                                    unsigned int From_ = j; 


                                    for(int j2=0;j2< Sample[SubSet[i]].vertex_List[j].edge_list.size();j2++)
                                    {
                                        
                                        unsigned int EdgeID = Sample[SubSet[i]].vertex_List[j].edge_list[j2].EdgeId; 
                                        To_ = Sample[SubSet[i]].vertex_List[j].edge_list[j2].VertexId;

                                        if(!((((IsGFLE[SubSet[i]][EdgeID]&GreaterArrow)!=0)&&(To_ > From_))||(((IsGFLE[SubSet[i]][EdgeID]&LessArrow)!=0)&&((To_ < From_)))))
                                        {
                                            ancestorEdgeN    = ancestorEdgeN+1;
                                            ancestorEdgeID   = EdgeID;
                                            ancestorVertexID = To_;
                                        }
                                    }

                                    if(ancestorEdgeN==1)
                                    {
                                        unsigned int originalEdgeID = IsGFLE[SubSet[i]][ancestorEdgeID];

                                        if(From_ > ancestorVertexID)
                                        {
                                            IsGFLE[SubSet[i]][ancestorEdgeID] = IsGFLE[SubSet[i]][ancestorEdgeID]|GreaterArrow;
                                        }else
                                        {
                                            IsGFLE[SubSet[i]][ancestorEdgeID] = IsGFLE[SubSet[i]][ancestorEdgeID]|LessArrow;
                                        }

       
                                        if(originalEdgeID!=IsGFLE[SubSet[i]][ancestorEdgeID])
                                        AddedleafN=AddedleafN+1;
                                    }

                                 }
                             }
                          } // Tag the general forward leaf edge in each graph of transaction graphs;




                        









                          for(int j=0;j<Sample[SubSet[i]].vertex_List.size();j++)
                          {

                           PDFS cur;
                           cur.ID = SubSet[i]; 

                           for(int j2=0;j2< Sample[SubSet[i]].vertex_List[j].edge_list.size();j2++) 
                           {
                            // if( Sample[i].vertex_List[j].VertexId<Sample[i].vertex_List[j].edge_list[j2].VertexId)
                            // {
                                cur.StartID = j;
                                cur.EndID   = Sample[SubSet[i]].vertex_List[j].edge_list[j2].VertexId; 

                                PartialOrderKey key;

                                
                                key.from       = 0;
                                key.to         = 1;
                                key.vlabelFrom = Sample[SubSet[i]].vertex_List[cur.StartID].vlabel;
                                key.vlabelTo   = Sample[SubSet[i]].vertex_List[cur.EndID].vlabel;
                                key.elabel     = Sample[SubSet[i]].vertex_List[cur.StartID].edge_list[j2].elabel;


                               if(new_projected.find(key)!=new_projected.end())
                              {
                                   new_projected[key].push_back(cur);

                              }else{

                                  std::vector<PDFS> entry; 
                                  entry.push_back(cur);
                                  assert(key.from!= key.to);
                                  new_projected.insert(make_pair(key,entry)); 
                               }  
                            }
                         }
                      }  





       }else{

                      for (int i = 0;i<Sample.size();i++) 
                      { 
                     

                          if(IsGFLE[i].size()==0)
                          {
                             IsGFLE[i].resize(Sample[i].EdgeSize);

                             for(int i1=0;i1<IsGFLE[i].size();i1++)
                             {
                                  IsGFLE[i][i1] = 0;
                             }


                             unsigned int  AddedleafN = 0;

                             for(int j=0;j<Sample[i].vertex_List.size();j++)
                             {

                                if(Sample[i].vertex_List[j].edge_list.size()==1)
                                {
                                    unsigned int To_   =  j;
                                    unsigned int From_ =  Sample[i].vertex_List[j].edge_list[0].VertexId;  


                                    if((To_ > From_))
                                    {
                                        IsGFLE[i][Sample[i].vertex_List[j].edge_list[0].EdgeId] = GreaterArrow;
                                    }else
                                    {
                                        IsGFLE[i][Sample[i].vertex_List[j].edge_list[0].EdgeId] = LessArrow;
                                    }

                                    AddedleafN = AddedleafN+1;
                                }

                             }

                             while(AddedleafN!=0)
                             {
                                  
                                  AddedleafN = 0;


                                 for(int j=0;j<Sample[i].vertex_List.size();j++)
                                 {

                                    unsigned int ancestorEdgeN   = 0; 
                                    unsigned int ancestorEdgeID  = MAX_UINT_CONSTANT;
                                    unsigned int ancestorVertexID = MAX_UINT_CONSTANT;
                                    unsigned int To_  =  MAX_UINT_CONSTANT;
                                    unsigned int From_ = j; 


                                    for(int j2=0;j2< Sample[i].vertex_List[j].edge_list.size();j2++)
                                    {
                                        
                                        unsigned int EdgeID = Sample[i].vertex_List[j].edge_list[j2].EdgeId; 
                                        To_ = Sample[i].vertex_List[j].edge_list[j2].VertexId;

                                        if(!((((IsGFLE[i][EdgeID]&GreaterArrow)!=0)&&(To_ > From_))||(((IsGFLE[i][EdgeID]&LessArrow)!=0)&&((To_ < From_)))))
                                        {
                                            ancestorEdgeN    = ancestorEdgeN+1;
                                            ancestorEdgeID   = EdgeID;
                                            ancestorVertexID = To_;
                                        }
                                    }

                                    if(ancestorEdgeN==1)
                                    {
                                        unsigned int originalEdgeID = IsGFLE[i][ancestorEdgeID];

                                        if(From_ > ancestorVertexID)
                                        {
                                            IsGFLE[i][ancestorEdgeID] = IsGFLE[i][ancestorEdgeID]|GreaterArrow;
                                        }else
                                        {
                                            IsGFLE[i][ancestorEdgeID] = IsGFLE[i][ancestorEdgeID]|LessArrow;
                                        }

       
                                        if(originalEdgeID!=IsGFLE[i][ancestorEdgeID])
                                        AddedleafN=AddedleafN+1;
                                    }

                                 }
                             }
                          } // Tag the general forward leaf edge in each graph of transaction graphs;







                          for(int j=0;j<Sample[i].vertex_List.size();j++)
                          {

                           PDFS cur;
                           cur.ID = i; 

                           for(int j2=0;j2< Sample[i].vertex_List[j].edge_list.size();j2++) 
                           {
                            // if( Sample[i].vertex_List[j].VertexId<Sample[i].vertex_List[j].edge_list[j2].VertexId)
                            // {
                                cur.StartID = j;
                                cur.EndID   = Sample[i].vertex_List[j].edge_list[j2].VertexId; 

                                PartialOrderKey key;

                                
                                key.from       = 0;
                                key.to         = 1;
                                key.vlabelFrom = Sample[i].vertex_List[cur.StartID].vlabel;
                                key.vlabelTo   = Sample[i].vertex_List[cur.EndID].vlabel;
                                key.elabel     = Sample[i].vertex_List[cur.StartID].edge_list[j2].elabel;


                               if(new_projected.find(key)!=new_projected.end())
                              {
                                   new_projected[key].push_back(cur);

                              }else{

                                  std::vector<PDFS> entry; 
                                  entry.push_back(cur);
                                  assert(key.from!= key.to);
                                  new_projected.insert(make_pair(key,entry)); 
                               }  
                            }
                         }
                      }  

       }


   


  

    for(Projected_map::reverse_iterator cur = new_projected.rbegin();cur != new_projected.rend(); ++cur){
       
       unsigned int Population;

       Support(Population,cur->second);

        if(Population>minsup)
        {
             if(Is_minimum(MAX_UINT_CONSTANT,cur->first.to,cur->first.from,cur->first.vlabelFrom,cur->first.vlabelTo,cur->first.elabel,false)==false)
             {
                continue;
             }

             DFSpath.push(Population,cur->first.elabel,cur->first.vlabelFrom,cur->first.vlabelTo,cur->second,MAX_UINT_CONSTANT,cur->first.from,cur->first.to);
             OccurringTransitive.resize(OccurringTransitive.size()+1);
             OccurringTransitive[OccurringTransitive.size()-1] = false; 
        }
    }


     while(DFSpath.size())
     { 



            std::vector<bool>          rightMostPath;
            unsigned int MaxNode;
           



        


                DFS * DFSset = &DFSpath[DFSpath.size()-1];
                MaxNode = std::max(DFSset->from,DFSset->to);


                     if( DFSset->isTraveled == false )
                     {
                         DFSset->isTraveled = true;
                     }else
                     {
                         if(DFSset->rootID==MAX_UINT_CONSTANT)
                         {
                            PartialOrderKeyThin Entry; 
                            Entry.vlabelTo = DFSset->vlabelTo;
                            Entry.elabel = DFSset->elabel;
                            Entry.vlabelFrom = DFSset->vlabelFrom;
                            EdgeFilterset.insert(Entry);
                         }


                      unsigned int rootOccurrenceID=DFSpath.size()-1;
                      unsigned int rootID;




                      if(OccurringTransitive[rootOccurrenceID]==true)
                      {
                        
                         rootID = DFSset->rootID;

                         while(DFSset->rootID==rootID)
                        {
                             DFSpath.pop();
                             OccurringTransitive.resize(OccurringTransitive.size()-1);
                             DFSset = &DFSpath[DFSpath.size()-1];
                         }

                     }
                      else{

                         DFSpath.pop();
                         OccurringTransitive.resize(OccurringTransitive.size()-1);
                     }

                        continue;
                     }




                new_projected.clear();
                new_projected_count.clear();


                {   

                    rightMostPath.resize(std::max(DFSset->from,DFSset->to)+1);
                    std::fill(rightMostPath.begin(),rightMostPath.end(),false);
                    rightMostPath[std::max(DFSset->from,DFSset->to)] = true;
                    unsigned int OldRmpV = std::max(DFSset->from,DFSset->to);
                    unsigned int rootID;
                
               
                    DFS * ParentDFSset = DFSset;
                    rootID = ParentDFSset->rootID;
     

                        while((rootID!=MAX_UINT_CONSTANT))
                        {
                    

                             if( ParentDFSset->from < ParentDFSset->to)
                             {
                                 if(ParentDFSset->to==OldRmpV)
                                 {
                                    OldRmpV =ParentDFSset->from;
                                 }
                                rightMostPath[OldRmpV] = true;
                             }
                        
                          
                            ParentDFSset = &DFSpath[rootID];
                            rootID = ParentDFSset->rootID;

                        };
                        if(rootID==MAX_UINT_CONSTANT)
                        {
                                if( ParentDFSset->from < ParentDFSset->to)
                                {
                                    if(ParentDFSset->to==OldRmpV)
                                    {
                                        OldRmpV =ParentDFSset->from;
                                    }
                                        rightMostPath[OldRmpV] = true;
                                }
                        }

                }





                    



                for (unsigned int n = 0; n < DFSset->Projected.size(); ++n)
                {
                    PDFS *cur = &DFSset->Projected[n];

                    unsigned int rootID   =   DFSset->rootID;
                    unsigned int parentID =   cur->parentID;
                    std::vector<bool> EdgeTraveled;
                    EdgeTraveled.resize(Sample[cur->ID].EdgeSize); 
                    std::fill(EdgeTraveled.begin(), EdgeTraveled.end(),false);
                    std::vector< unsigned int > DPMap;
                    DPMap.resize(Sample[cur->ID].vertex_List.size()+1);
                    std::fill(DPMap.begin(),DPMap.end(),MAX_UINT_CONSTANT);
               


                    DPMap[cur->StartID] = DFSset->from;
                    DPMap[cur->EndID] =  DFSset->to;



                    DFS * ParentDFSset = NULL;
                    PDFS * Parentcur = NULL;

                    {


                            Parentcur =cur;
                            ParentDFSset=DFSset;
                            rootID = ParentDFSset->rootID;
                            parentID = Parentcur->parentID;


                            while(rootID!=MAX_UINT_CONSTANT)
                            {
                            
                                unsigned int to = Parentcur->EndID;
                                unsigned int from = Parentcur->StartID;

                               
                                DPMap[from] = ParentDFSset->from;
                                DPMap[to] = ParentDFSset->to;

                                {
                                    for(int i=0;i<Sample[Parentcur->ID].vertex_List[from].edge_list.size();i++)  
                                    { 
                                     
                                         if( Sample[Parentcur->ID].vertex_List[from].edge_list[i].VertexId == to )
                                         if( (Sample[Parentcur->ID].vertex_List[from].edge_list[i].elabel==ParentDFSset->elabel)&&\
                                            (Sample[Parentcur->ID].vertex_List[from].vlabel == ParentDFSset->vlabelFrom)&&\
                                            (Sample[Parentcur->ID].vertex_List[to].vlabel == ParentDFSset->vlabelTo ))
                                        {
                                             EdgeTraveled[ Sample[Parentcur->ID].vertex_List[from].edge_list[i].EdgeId ]=true; 
                                        }

                                    }
                                }

                                ParentDFSset = &DFSpath[rootID];
                                Parentcur = &(ParentDFSset->Projected[parentID]);
                                rootID = ParentDFSset->rootID;
                                parentID = Parentcur->parentID;
                                
                             };

                             if(rootID==MAX_UINT_CONSTANT)
                             {
                                unsigned int to = Parentcur->EndID;
                                unsigned int from = Parentcur->StartID;

                               
                                DPMap[from] = ParentDFSset->from;
                                DPMap[to] = ParentDFSset->to;

                                {
                                    for(int i=0;i<Sample[Parentcur->ID].vertex_List[from].edge_list.size();i++)  
                                    { 
                                     
                                         if( Sample[Parentcur->ID].vertex_List[from].edge_list[i].VertexId == to )
                                         if( (Sample[Parentcur->ID].vertex_List[from].edge_list[i].elabel==ParentDFSset->elabel)&&\
                                            (Sample[Parentcur->ID].vertex_List[from].vlabel == ParentDFSset->vlabelFrom)&&\
                                            (Sample[Parentcur->ID].vertex_List[to].vlabel == ParentDFSset->vlabelTo ))
                                        {
                                             EdgeTraveled[ Sample[Parentcur->ID].vertex_List[from].edge_list[i].EdgeId ]=true; 
                                        }

                                    }
                                }

                             }

                        }
         
                {

                  

                   for( int j = 0;j<Sample[cur->ID].vertex_List.size();j++ )
                   {


                       for(int i=0;i< Sample[cur->ID].vertex_List[j].edge_list.size();i++)
                       { 
                          
                          Edge * temp = &Sample[cur->ID].vertex_List[j].edge_list[i];
                    
                          if( ( EdgeTraveled[temp->EdgeId]==false )&&( (DPMap[j]!=MAX_UINT_CONSTANT)) )
                          {

                               PartialOrderKey key;
                               unsigned int leaf; 
                               bool Is_Right_Most = false; 



                               if((DPMap[temp->VertexId]==MAX_UINT_CONSTANT))
                               {

                                     key.to =         MaxNode+1;
                                     key.from =       DPMap[j];
                                     leaf =           temp->VertexId;
                                     key.elabel =     temp->elabel;
                                     key.vlabelTo =   Sample[cur->ID].vertex_List[temp->VertexId].vlabel; 
                                     key.vlabelFrom = Sample[cur->ID].vertex_List[j].vlabel; 
                                
                                  


                                     if(((key.to > key.from)&&(rightMostPath[key.from]==true)))
                                     {

                                         if(new_projected.find(key)!=new_projected.end())
                                          {

                                                PDFS curEntry; 
                                                curEntry.ID =                  cur->ID;
                                                curEntry.StartID =                   j;
                                                curEntry.EndID  =       temp->VertexId;
                                                curEntry.parentID =                  n;
                                                new_projected[key].push_back(curEntry);

                                          }else{

                                                std::vector<PDFS> entry; 
                                                entry.resize(1);

                                                entry[0].ID =              cur->ID;
                                                entry[0].StartID =               j;
                                                entry[0].EndID =    temp->VertexId;
                                                entry[0].parentID =              n;


                                                assert(key.from!= key.to);
                                                new_projected.insert(make_pair(key,entry)); 
                                         } 

                                         Is_Right_Most = true;

                                    }//right most extended checking 

                               }else{

                                     key.to =         DPMap[temp->VertexId];
                                     key.from =       DPMap[j];
                                     leaf =           temp->VertexId;
                                     key.elabel =     temp->elabel;
                                     key.vlabelTo =   Sample[cur->ID].vertex_List[temp->VertexId].vlabel; 
                                     key.vlabelFrom = Sample[cur->ID].vertex_List[j].vlabel;




                                    if( key.to > key.from )
                                    {
                                        continue;
                                    }

                                    if((( key.to < key.from )&&(key.from == MaxNode)&&(rightMostPath[key.to]==true)))
                                    {

                                         if(new_projected.find(key)!=new_projected.end())
                                          {

                                                PDFS curEntry; 
                                                curEntry.ID =                  cur->ID;
                                                curEntry.StartID =                   j;
                                                curEntry.EndID  =       temp->VertexId;
                                                curEntry.parentID =                  n;
                                                new_projected[key].push_back(curEntry);

                                          }else{

                                                std::vector<PDFS> entry; 
                                                entry.resize(1);

                                                entry[0].ID =              cur->ID;
                                                entry[0].StartID =               j;
                                                entry[0].EndID =    temp->VertexId;
                                                entry[0].parentID =              n;
                                                assert(key.from!= key.to);
                                                new_projected.insert(make_pair(key,entry)); 
                                         } 
 
                                          Is_Right_Most = true;
                                          
                                    }//right most extended checking 

                               }




                          
           

                             if(Is_Right_Most)
                             { 
                                continue;
                             }





                             if(new_projected_count.find(key)!=new_projected_count.end())
                              {

                                     uint16_t index = new_projected_count[key];


                   

                                     if( FullextensionSupportContext[index].ID != cur->ID )
                                     {
                                        FullextensionSupportContext[index].ID = cur->ID;
                                        FullextensionSupportContext[index].Count = FullextensionSupportContext[index].Count+1;
                                     }

                                       if(Trim)
                                       {

                                            if(key.to<key.from)
                                            {
                                      
                                               if(FullextensionIsTransitiveContext[index] == true)
                                               {
                                                  if( FullextensionOccurrenceContext[index] == (n-1))
                                                  {
                                                    FullextensionOccurrenceContext[index] = n;
                                                  }

                                                  if( FullextensionOccurrenceContext[index] != n )
                                                  {
                                                    FullextensionIsTransitiveContext[index] = false;
                                                  }
                                              }
                                            }else if( Sample[cur->ID].vertex_List[leaf].edge_list.size()==1 )
                                             {
                                                  if(FullextensionIsTransitiveContext[index] == true)
                                                 {
                                                   if( FullextensionOccurrenceContext[index] == (n-1))
                                                   {
                                                      FullextensionOccurrenceContext[index] = n;
                                                   }

                                                  if( FullextensionOccurrenceContext[index] != n )
                                                  {
                                                    FullextensionIsTransitiveContext[index] = false;
                                                  }
                                                 }
                                             }


                                        }
 



                              }else{

                                       uint16_t index = (uint16_t)new_projected_count.size();
                                       new_projected_count.insert(std::make_pair(key,index));
                                       FullextensionSupportContext[index].ID = cur->ID;
                                       FullextensionSupportContext[index].Count = 1;


                                       FullextensionIsTransitiveContext[index] = false; 
                                       if(Trim)
                                       { 
                                            bool is_GeneralForwardLeaf = false;

                                            if(key.to<key.from)
                                            {
                                               FullextensionOccurrenceContext[index]=n;

                                               if(n!=0)
                                               {
                                                  FullextensionIsTransitiveContext[index] = false;
                                               }else
                                               {
                                                  FullextensionIsTransitiveContext[index] = true; 
                                               }

                                            } else if( Sample[cur->ID].vertex_List[leaf].edge_list.size()==1 )
                                            {
                                                 is_GeneralForwardLeaf = true;

                                            }else{


                                                for(int i3 = 0;i3< Sample[cur->ID].vertex_List[leaf].edge_list.size();i3++)
                                                {
                                                    


                                                    if((Sample[cur->ID].vertex_List[leaf].edge_list[i3].VertexId ==j)&&(Sample[cur->ID].vertex_List[leaf].edge_list[i3].elabel==temp->elabel))
                                                    {
                                                         unsigned int EdgeID = Sample[cur->ID].vertex_List[leaf].edge_list[i3].EdgeId;

                                                           if(( (leaf > j)&&((IsGFLE[cur->ID][EdgeID]&GreaterArrow)!=0) )||( (leaf < j)&&(IsGFLE[cur->ID][EdgeID]&LessArrow)!=0))
                                                          {
                                                              is_GeneralForwardLeaf = true;
                                                              break;
                                                          }

                                                    }

                                                }

                                            }


                                            if(is_GeneralForwardLeaf==true)
                                            {


                                                FullextensionOccurrenceContext[index]=n;

                                                if(n!=0)
                                                {
                                                  FullextensionIsTransitiveContext[index] = false;
                                                }else
                                                {
                                                  FullextensionIsTransitiveContext[index] = true; 
                                                }

                                            }



                                       }

                                   } 
                                 
                          }
                       }
                       
                    }

                }//The count summary from fully extended. 

            }

             unsigned int ancestorID; 
             ancestorID = DFSpath.size()-1;
             bool isReported = true; 
             bool isTrim = false;
             unsigned int Population;


             {
                for(int i=0;i< new_projected_count.size();i++ ) 
                {

                    if(DFSset->support == FullextensionSupportContext[i].Count)  
                    {
                        isReported = false; 
                    }

                }
             }



        
        if(Trim)
        {
                for(Projected_count_map::iterator it = new_projected_count.begin();it!=new_projected_count.end();++it)
                {

                 
                         if ((FullextensionIsTransitiveContext[it->second]==true)&&(FullextensionOccurrenceContext[it->second]==(DFSset->Projected.size()-1)))
                        {  

                            /* std::vector<unsigned int> keyset;
                             keyset.resize(0);

                             Is_minimum(ancestorID,(it->first).to,(it->first).from,(it->first).vlabelFrom,(it->first).vlabelTo,(it->first).elabel,true);


                                  for(int i=minimumGraphStacksize;i>=0;i--)
                                  {
                                     keyset.resize(keyset.size()+1);
                                     keyset[keyset.size()-1] = minimumCodeTrace[i].to;
                                     keyset.resize(keyset.size()+1);
                                     keyset[keyset.size()-1] = minimumCodeTrace[i].from;
                                     keyset.resize(keyset.size()+1);
                                     keyset[keyset.size()-1] = ((minimumCodeTrace[i].vlabelTo<<16)|(minimumCodeTrace[i].vlabelFrom));
                                     keyset.resize(keyset.size()+1);
                                     keyset[keyset.size()-1] = minimumCodeTrace[i].elabel;
                                  }
                                  if( filterset.find(keyset)!=filterset.end())
                                  {
                                    isTrim = true;
                                    break;
                                  }*/ 
                              // The maintenance of the history list is not needed.

                               isTrim = true;    
                        }
                    
              }
        }


            if(isTrim)
            {
              continue;
            }

            

            for(Projected_map::reverse_iterator cur = new_projected.rbegin();cur != new_projected.rend(); ++cur){



                   DFSset=&DFSpath[ancestorID];
                   Support(Population,cur->second);

                   if( Population == DFSset->support )
                   {
                     isReported = false; 
                   } 


                   {
                        PartialOrderKeyThin Entry; 
                        Entry.vlabelTo =   cur->first.vlabelTo;
                        Entry.elabel =     cur->first.elabel;
                        Entry.vlabelFrom = cur->first.vlabelFrom;

                        if(EdgeFilterset.find(Entry) != EdgeFilterset.end())
                        {
                            continue;
                        }


                   }

             
                  

                   if(Population>minsup){


                      
                        if(Is_minimum(ancestorID,cur->first.to,cur->first.from,cur->first.vlabelFrom,cur->first.vlabelTo,cur->first.elabel,false)==false)
                        {

                            continue;

                        }else
                        {  
                           /* if(Trim)
                            {
                                  std::vector<unsigned int> keyset;
                                  keyset.resize(0);

                                  for(int i=minimumGraphStacksize;i>=0;i--)
                                  {
                                     keyset.resize(keyset.size()+1);
                                     keyset[keyset.size()-1] = minimumGraphContext[i].to;
                                     keyset.resize(keyset.size()+1);
                                     keyset[keyset.size()-1] = minimumGraphContext[i].from;
                                     keyset.resize(keyset.size()+1);
                                     keyset[keyset.size()-1] = (minimumGraphContext[i].vlabelTo<<16)|(minimumGraphContext[i].vlabelFrom);
                                     keyset.resize(keyset.size()+1);
                                     keyset[keyset.size()-1] = minimumGraphContext[i].elabel;
                                  }

                                  filterset.insert(keyset);
                            }*/
                           // The maintenance of the history list is not needed.

                        }




                    if(Population == DFSset->support)
                    {
                        if( cur->first.to < cur->first.from )
                       {
                           unsigned int OccuringMappingNum;

                           OccurringMapping(OccuringMappingNum,cur->second);
                
                           if(OccuringMappingNum < DFSset->Projected.size())
                           {
                              OccurringTransitive.resize(OccurringTransitive.size()+1);
                              OccurringTransitive[OccurringTransitive.size()-1] = false; 
                           }else
                           {     
                             OccurringTransitive.resize(OccurringTransitive.size()+1);
                             OccurringTransitive[OccurringTransitive.size()-1] = true; 
                             assert(OccuringMappingNum == DFSset->Projected.size());
                           }

                      }else if(isLeaf(cur->second,DFSset->elabel))
                      {

                           unsigned int OccuringMappingNum; 

                           OccurringMapping(OccuringMappingNum,cur->second);
                          
                           if(OccuringMappingNum < DFSset->Projected.size())
                           {
                              OccurringTransitive.resize(OccurringTransitive.size()+1);
                              OccurringTransitive[OccurringTransitive.size()-1] = false; 
                           }
                            else
                           {     
                             OccurringTransitive.resize(OccurringTransitive.size()+1);
                             OccurringTransitive[OccurringTransitive.size()-1] = true; 
                             assert(OccuringMappingNum == DFSset->Projected.size());
                          } 

         
                       }else{

                            OccurringTransitive.resize(OccurringTransitive.size()+1);
                            OccurringTransitive[OccurringTransitive.size()-1] = false; 
                        } 

                    }else
                    {
                          OccurringTransitive.resize(OccurringTransitive.size()+1);
                          OccurringTransitive[OccurringTransitive.size()-1] = false; 
                    }

   
                      DFSpath.push(Population,cur->first.elabel,cur->first.vlabelFrom,cur->first.vlabelTo,cur->second,ancestorID,cur->first.from,cur->first.to);
                   }
            }


     


            


           


            if(isReported)
            {

          
                Graph_ * GraphEntry; 


                FrequentGraphs.resize(FrequentGraphs.size()+1);
                GraphEntry=&FrequentGraphs[FrequentGraphs.size()-1];


                   // GraphEntry->
                {
                    unsigned int rootID;
                    unsigned int Edge_Index = 0;
                    unsigned int vertex_Index = 0; 
                    std::vector<unsigned int> vertex_IndexList; 


                    vertex_IndexList.resize(std::max(DFSset->to,DFSset->from)+1);
                    std::fill(vertex_IndexList.begin(),vertex_IndexList.end(),MAX_UINT_CONSTANT);




                    for(unsigned int i=0;i<rightMostPath.size();i++)
                    {
                        if(rightMostPath[i]== true)
                        {
                            vertex_IndexList[i] = vertex_Index;
                            vertex_Index = vertex_Index+1;
                        }
                    }


                    GraphEntry->vertex_List.resize(vertex_Index);
                    DFS * ParentDFSset= &DFSpath[ancestorID];
                    rootID = ParentDFSset->rootID;
                    
                    while(rootID!=MAX_UINT_CONSTANT)
                    {
                    
                        int from = vertex_IndexList[ParentDFSset->from];
                        int to   = vertex_IndexList[ParentDFSset->to];
                        if(from == MAX_UINT_CONSTANT)
                        {
                            vertex_IndexList[ParentDFSset->from] = vertex_Index; 
                            from = vertex_IndexList[ParentDFSset->from];
                            GraphEntry->vertex_List.resize(GraphEntry->vertex_List.size()+1);
                            vertex_Index = vertex_Index+1;
                        }
                        if(to == MAX_UINT_CONSTANT) 
                        {
                            vertex_IndexList[ParentDFSset->to] = vertex_Index; 
                            to = vertex_IndexList[ParentDFSset->to];
                            GraphEntry->vertex_List.resize(GraphEntry->vertex_List.size()+1);
                            vertex_Index = vertex_Index+1;
                        }
          

                        GraphEntry->vertex_List[from].vlabel = ParentDFSset->vlabelFrom;
                        GraphEntry->vertex_List[to].vlabel   = ParentDFSset->vlabelTo;
                        GraphEntry->vertex_List[from].edge_list.resize(GraphEntry->vertex_List[from].edge_list.size()+1);
                        GraphEntry->vertex_List[from].edge_list[GraphEntry->vertex_List[from].edge_list.size()-1].VertexId = to;
                        GraphEntry->vertex_List[from].edge_list[GraphEntry->vertex_List[from].edge_list.size()-1].elabel =   ParentDFSset->elabel;
                        GraphEntry->vertex_List[from].edge_list[GraphEntry->vertex_List[from].edge_list.size()-1].EdgeId =   Edge_Index ;                                                                           
                        ParentDFSset = &DFSpath[rootID];
                        rootID = ParentDFSset->rootID;
                        Edge_Index = Edge_Index+1;
                        
                    }; 
                    if(rootID==MAX_UINT_CONSTANT)
                    {
                        int from = vertex_IndexList[ParentDFSset->from];
                        int to   = vertex_IndexList[ParentDFSset->to];
                        if(from == MAX_UINT_CONSTANT)
                        {
                            vertex_IndexList[ParentDFSset->from] = vertex_Index; 
                            from = vertex_IndexList[ParentDFSset->from];
                            GraphEntry->vertex_List.resize(GraphEntry->vertex_List.size()+1);
                            vertex_Index = vertex_Index+1;
                        }
                        if(to == MAX_UINT_CONSTANT) 
                        {
                            vertex_IndexList[ParentDFSset->to] = vertex_Index; 
                            to = vertex_IndexList[ParentDFSset->to];
                            GraphEntry->vertex_List.resize(GraphEntry->vertex_List.size()+1);
                            vertex_Index = vertex_Index+1;
                        }
                

                        GraphEntry->vertex_List[from].vlabel = ParentDFSset->vlabelFrom;
                        GraphEntry->vertex_List[to].vlabel   = ParentDFSset->vlabelTo;
                        GraphEntry->vertex_List[from].edge_list.resize(GraphEntry->vertex_List[from].edge_list.size()+1);
                        GraphEntry->vertex_List[from].edge_list[GraphEntry->vertex_List[from].edge_list.size()-1].VertexId = to;
                        GraphEntry->vertex_List[from].edge_list[GraphEntry->vertex_List[from].edge_list.size()-1].elabel =   ParentDFSset->elabel;
                        GraphEntry->vertex_List[from].edge_list[GraphEntry->vertex_List[from].edge_list.size()-1].EdgeId =   Edge_Index ;                                                                           
                        Edge_Index = Edge_Index+1;
                    }

                       GraphEntry->EdgeSize = Edge_Index;
            }
                 
        }
    }

}

bool ComStruct::IsConteined(Graph_ &superGraph,Graph_ &subGraph)
{

    unsigned int CompareContext1_size = superGraph.EdgeSize ;
    unsigned int CompareContext2_size = subGraph.EdgeSize;

    if(CompareContext1_size < CompareContext2_size)
    {
        return false; 
    }



    for(int i=0;i<superGraph.vertex_List.size();i++)
    {
         for( int j =0;j<superGraph.vertex_List[i].edge_list.size();j++)
         {

            unsigned int labelA = superGraph.vertex_List[i].vlabel;
            unsigned int labelB = superGraph.vertex_List[superGraph.vertex_List[i].edge_list[j].VertexId].vlabel;
            unsigned EdgeId = superGraph.vertex_List[i].edge_list[j].EdgeId;

            CompareContext1[EdgeId]  = std::min(labelA,labelB);
            CompareContext1[EdgeId]  = CompareContext1[EdgeId] << 8;
            CompareContext1[EdgeId]  = CompareContext1[EdgeId]+std::max(labelA,labelB);
            CompareContext1[EdgeId]  = CompareContext1[EdgeId] << 8;
            CompareContext1[EdgeId] |= superGraph.vertex_List[i].edge_list[j].elabel;
         }
    }



     for(int i=0;i<subGraph.vertex_List.size();i++)
     {
         for( int j =0;j<subGraph.vertex_List[i].edge_list.size();j++)
         {

            unsigned int labelA = subGraph.vertex_List[i].vlabel;
            unsigned int labelB = subGraph.vertex_List[subGraph.vertex_List[i].edge_list[j].VertexId].vlabel;
            unsigned EdgeId = subGraph.vertex_List[i].edge_list[j].EdgeId;


            CompareContext2[EdgeId]  = std::min(labelA,labelB);
            CompareContext2[EdgeId]  = CompareContext2[EdgeId] << 8;
            CompareContext2[EdgeId]  = CompareContext2[EdgeId]+std::max(labelA,labelB);
            CompareContext2[EdgeId]  = CompareContext2[EdgeId] << 8;
            CompareContext2[EdgeId] |= subGraph.vertex_List[i].edge_list[j].elabel;
         }
     }

      std::sort(&CompareContext1[0],&CompareContext1[CompareContext1_size]);
      std::sort(&CompareContext2[0],&CompareContext2[CompareContext2_size]);
      
     {
            int i,j;

            for( i=0,j=0;i<CompareContext1_size&&j<CompareContext2_size;)
            {
                        {
                            if(CompareContext1[i]==CompareContext2[j])
                            {
                                i++;
                                j++;
                                continue;
                            }
                            if(CompareContext1[i]>CompareContext2[j])
                            {
                                return false;
                            }
                            if(CompareContext1[i]<CompareContext2[j])
                            {
                                i++;
                                continue;
                            }
                        }
               
            }


           if(graphMatch(superGraph,subGraph)==true)
           {
                return true;
           }else
           {
                return false; 
           }

     }
} 



bool ComStruct::OutPutGraphNode(Graph_ &g1,FILE * f1,unsigned int Index_)
{

        std::vector< unsigned int > CIndex;


        {
            CIndex.resize(0);
            CIndex.push_back(Index_);

             for( int j1 =0;j1<g1.vertex_List[Index_].edge_list.size();j1++)
            {
                unsigned int id2 =  g1.vertex_List[Index_].edge_list[j1].VertexId;
                CIndex.push_back(id2);
            }

            if(fprintf(f1,"%d\n",CIndex.size())<0)
                return false;
       

            for(int i2=0;i2<CIndex.size();i2++)
            {
                if(fprintf(f1,"%d\n",g1.vertex_List[CIndex[i2]].vlabel)<0)
                return false; 
            }

            if(fprintf(f1,"%d\n",CIndex.size()-1)<0)
                return false;


            for( int j1 =0;j1<g1.vertex_List[Index_].edge_list.size();j1++)
            {
                  unsigned int EdgeID; 

                  EdgeID = g1.vertex_List[Index_].edge_list[j1].EdgeId;
                  
                  {

                     unsigned int id1 = 0;
                     unsigned int id2 = 0;
                     unsigned int VertexId = g1.vertex_List[Index_].edge_list[j1].VertexId;

                     for(;id2<CIndex.size();id2++)
                     {
                        if(VertexId==CIndex[id2])
                        {
                            break;
                        }
                     }


                     if(id2 == CIndex.size())
                     {
                        continue;
                     }


                     unsigned int elabel =  g1.vertex_List[Index_].edge_list[j1].elabel; 

                     if(fprintf(f1,"%d ",id1)<0)
                        return false;

                     if(fprintf(f1,"%d ",id2)<0)
                        return false;

                     if(fprintf(f1,"%d\n",elabel)<0)
                        return false;

                  }
              }
           
         }

        return true; 
}

bool ComStruct::OutPutGraph(Graph_ &g1,FILE * f1)
{

         for(int i =0;i<g1.EdgeSize;i++)
        {
            CompareContext1[i]=0;
        }

         if(fprintf(f1,"%d\n",g1.vertex_List.size())<0)
            return false;

       for(int i1=0;i1<g1.vertex_List.size();i1++)
       {
          
         if(fprintf(f1,"%d\n",g1.vertex_List[i1].vlabel)<0)
            return false; 
       }

       for(int i =0;i<g1.EdgeSize;i++)
       {
            CompareContext1[i]=0;
       }


       if(fprintf(f1,"%d\n",g1.EdgeSize)<0)
        return false;


        for(int i1=0;i1<g1.vertex_List.size();i1++)
        {

               for( int j1 =0;j1<g1.vertex_List[i1].edge_list.size();j1++)
              {
                  unsigned int EdgeID; 

                  EdgeID = g1.vertex_List[i1].edge_list[j1].EdgeId;
                  if(CompareContext1[EdgeID]!=MAX_UINT_CONSTANT)
                  {

                     unsigned int id1 =  i1;
                     unsigned int id2 =  g1.vertex_List[i1].edge_list[j1].VertexId;
                     unsigned int elabel =  g1.vertex_List[i1].edge_list[j1].elabel; 

                     if(fprintf(f1,"%d ",std::min(id1,id2))<0)
                        return false;

                     if(fprintf(f1,"%d ",std::max(id1,id2))<0)
                        return false;

                     if(fprintf(f1,"%d\n",elabel)<0)
                        return false;

                    CompareContext1[EdgeID] = MAX_UINT_CONSTANT;
                  }
             
              }
        }

        return true; 
}





bool ComStruct::graphMatch(Graph_ &g1,Graph_ &g2)
{

       DFSpath.clear();
       GraphPair.resize(2);
       new_projected.clear();


       if(isTrace==true)
       {
         TraceG1.resize(0);
         TraceG2.resize(0);
       }


       GraphPair[0].vertex_List.resize(g1.vertex_List.size());
       GraphPair[1].vertex_List.resize(g2.vertex_List.size());



       for(int i1=0;i1<g1.vertex_List.size();i1++)
       {
           GraphPair[0].vertex_List[i1].vlabel  = g1.vertex_List[i1].vlabel;
           GraphPair[0].vertex_List[i1].edge_list.resize(0);
       }



       for(int i1=0;i1<g2.vertex_List.size();i1++)
       {
           GraphPair[1].vertex_List[i1].vlabel  = g2.vertex_List[i1].vlabel;
           GraphPair[1].vertex_List[i1].edge_list.resize(0);
       }

       for(int i =0;i<g1.EdgeSize;i++)
       {
            CompareContext1[i]=0;
       }

       for(int i =0;i<g2.EdgeSize;i++)
       {
            CompareContext2[i]=0;
       }



        for(int i1=0;i1<g1.vertex_List.size();i1++)
        {

               for( int j1 =0;j1<g1.vertex_List[i1].edge_list.size();j1++)
              {
                  unsigned int EdgeID; 

                  EdgeID = g1.vertex_List[i1].edge_list[j1].EdgeId;
                  if(CompareContext1[EdgeID]!=MAX_UINT_CONSTANT)
                  {
                    unsigned int id1 =  i1;
                    unsigned int id2 =  g1.vertex_List[i1].edge_list[j1].VertexId;
       

                    GraphPair[0].vertex_List[id1].edge_list.resize(GraphPair[0].vertex_List[id1].edge_list.size()+1);
                    GraphPair[0].vertex_List[id1].edge_list[GraphPair[0].vertex_List[id1].edge_list.size()-1].VertexId = id2;
                    GraphPair[0].vertex_List[id1].edge_list[GraphPair[0].vertex_List[id1].edge_list.size()-1].elabel  = g1.vertex_List[i1].edge_list[j1].elabel;
                    GraphPair[0].vertex_List[id1].edge_list[GraphPair[0].vertex_List[id1].edge_list.size()-1].EdgeId  = g1.vertex_List[i1].edge_list[j1].EdgeId;


                    GraphPair[0].vertex_List[id2].edge_list.resize(GraphPair[0].vertex_List[id2].edge_list.size()+1);
                    GraphPair[0].vertex_List[id2].edge_list[GraphPair[0].vertex_List[id2].edge_list.size()-1].VertexId = id1;
                    GraphPair[0].vertex_List[id2].edge_list[GraphPair[0].vertex_List[id2].edge_list.size()-1].elabel  = g1.vertex_List[i1].edge_list[j1].elabel;
                    GraphPair[0].vertex_List[id2].edge_list[GraphPair[0].vertex_List[id2].edge_list.size()-1].EdgeId =  g1.vertex_List[i1].edge_list[j1].EdgeId;



                    CompareContext1[EdgeID] = MAX_UINT_CONSTANT;
                  }
             
              }
        }

       for(int i1=0;i1<g2.vertex_List.size();i1++)
        {

               for( int j1 =0;j1<g2.vertex_List[i1].edge_list.size();j1++)
              {
                  unsigned int EdgeID; 

                  EdgeID = g2.vertex_List[i1].edge_list[j1].EdgeId;
                  if(CompareContext2[EdgeID]!=MAX_UINT_CONSTANT)
                  {
                    unsigned int id1 =  i1;
                    unsigned int id2 =  g2.vertex_List[i1].edge_list[j1].VertexId;
       

                    GraphPair[1].vertex_List[id1].edge_list.resize(GraphPair[1].vertex_List[id1].edge_list.size()+1);
                    GraphPair[1].vertex_List[id1].edge_list[GraphPair[1].vertex_List[id1].edge_list.size()-1].VertexId = id2;
                    GraphPair[1].vertex_List[id1].edge_list[GraphPair[1].vertex_List[id1].edge_list.size()-1].elabel  = g2.vertex_List[i1].edge_list[j1].elabel;
                    GraphPair[1].vertex_List[id1].edge_list[GraphPair[1].vertex_List[id1].edge_list.size()-1].EdgeId  = g2.vertex_List[i1].edge_list[j1].EdgeId;

                    GraphPair[1].vertex_List[id2].edge_list.resize(GraphPair[1].vertex_List[id2].edge_list.size()+1);
                    GraphPair[1].vertex_List[id2].edge_list[GraphPair[1].vertex_List[id2].edge_list.size()-1].VertexId = id1;
                    GraphPair[1].vertex_List[id2].edge_list[GraphPair[1].vertex_List[id2].edge_list.size()-1].elabel  = g2.vertex_List[i1].edge_list[j1].elabel;
                    GraphPair[1].vertex_List[id2].edge_list[GraphPair[1].vertex_List[id2].edge_list.size()-1].EdgeId  = g2.vertex_List[i1].edge_list[j1].EdgeId;

                    CompareContext2[EdgeID] = MAX_UINT_CONSTANT;
                  }
             
              }
        }

        GraphPair[0].EdgeSize = g1.EdgeSize;
        GraphPair[1].EdgeSize = g2.EdgeSize;



       for (int i = 0;i<GraphPair.size();i++) 
      { 
          for(int j=0;j<GraphPair[i].vertex_List.size();j++)
          {

           PDFS cur;
           cur.ID = i; 

           for(int j2=0;j2< GraphPair[i].vertex_List[j].edge_list.size();j2++) 
           {
            // if( GraphPair[i].vertex_List[j].VertexId<GraphPair[i].vertex_List[j].edge_list[j2].VertexId)
            // {
                cur.StartID = j;
                cur.EndID   = GraphPair[i].vertex_List[j].edge_list[j2].VertexId; 

                PartialOrderKey key;

                
                key.from       = 0;
                key.to         = 1;
                key.vlabelFrom = GraphPair[i].vertex_List[cur.StartID].vlabel;
                key.vlabelTo   = GraphPair[i].vertex_List[cur.EndID].vlabel;
                key.elabel     = GraphPair[i].vertex_List[cur.StartID].edge_list[j2].elabel;


               if(new_projected.find(key)!=new_projected.end())
              {
                   new_projected[key].push_back(cur);

              }else{

                  std::vector<PDFS> entry; 
                  entry.push_back(cur);
                  assert(key.from!= key.to);
                  new_projected.insert(make_pair(key,entry)); 
               }  
            }
         }
      }  


  

    for(Projected_map::reverse_iterator cur = new_projected.rbegin();cur != new_projected.rend(); ++cur){
       
       unsigned int Population;

       Support(Population,cur->second);

        if(Population>1)
        {
             DFSpath.push(Population,cur->first.elabel,cur->first.vlabelFrom,cur->first.vlabelTo,cur->second,MAX_UINT_CONSTANT,cur->first.from,cur->first.to);
        }
    }


     while(DFSpath.size())
     { 



            std::vector<bool>          rightMostPath;
            std::vector<rootComposeID>   rootIDArray;
         



                    DFS * DFSset = &DFSpath[DFSpath.size()-1];
                   

                    if( DFSset->isTraveled == false )
                     {
                         DFSset->isTraveled = true;
                     }else
                     {
                         DFSpath.pop();
                         continue;
                     }



                    new_projected.clear();
                    rightMostPath.resize(std::max(DFSset->from,DFSset->to)+1);
                    std::fill(rightMostPath.begin(),rightMostPath.end(),false);
                    rightMostPath[std::max(DFSset->from,DFSset->to)] = true;
                    unsigned int OldRmpV = std::max(DFSset->from,DFSset->to);
                    unsigned int rootID;
                



                    DFS * ParentDFSset = DFSset;
                    rootID = ParentDFSset->rootID;

                        while((rootID!=MAX_UINT_CONSTANT))
                        {
                    

                             if( ParentDFSset->from < ParentDFSset->to)
                             {
                                 if(ParentDFSset->to==OldRmpV)
                                 {
                                    OldRmpV =ParentDFSset->from;
                                 }
                                rightMostPath[OldRmpV] = true;
                             }
                        
                          
                            ParentDFSset = &DFSpath[rootID];
                            rootID = ParentDFSset->rootID;

                        };//To find rightmost path. 
                        if(rootID==MAX_UINT_CONSTANT)
                        {
                                if( ParentDFSset->from < ParentDFSset->to)
                                {
                                    if(ParentDFSset->to==OldRmpV)
                                    {
                                        OldRmpV =ParentDFSset->from;
                                    }
                                        rightMostPath[OldRmpV] = true;
                                }
                        }


                




                for (unsigned int n = 0; n < DFSset->Projected.size(); ++n)
                {
                    PDFS *cur = &DFSset->Projected[n];

                    unsigned int rootID   =   DFSset->rootID;
                    unsigned int parentID =   cur->parentID;
                    std::vector<bool> EdgeTraveled;
                    EdgeTraveled.resize(GraphPair[cur->ID].EdgeSize); 
                    std::fill(EdgeTraveled.begin(), EdgeTraveled.end(),false);
                    std::vector< unsigned int > DPMap;
                    DPMap.resize(GraphPair[cur->ID].vertex_List.size()+1);
                    std::fill(DPMap.begin(),DPMap.end(),MAX_UINT_CONSTANT);
               


                    DPMap[cur->StartID] = DFSset->from;
                    DPMap[cur->EndID] =  DFSset->to;



                    DFS * ParentDFSset = NULL;
                    PDFS * Parentcur = NULL;

                    {


                            Parentcur =cur;
                            ParentDFSset=DFSset;
                            rootID = ParentDFSset->rootID;
                            parentID = Parentcur->parentID;


                            while(rootID!=MAX_UINT_CONSTANT)
                            {
                            
                                unsigned int to = Parentcur->EndID;
                                unsigned int from = Parentcur->StartID;

                               
                                DPMap[from] = ParentDFSset->from;
                                DPMap[to] = ParentDFSset->to;

                                {
                                    for(int i=0;i<GraphPair[Parentcur->ID].vertex_List[from].edge_list.size();i++)  
                                    { 
                                     
                                         if( GraphPair[Parentcur->ID].vertex_List[from].edge_list[i].VertexId == to )
                                         if( (GraphPair[Parentcur->ID].vertex_List[from].edge_list[i].elabel==ParentDFSset->elabel)&&\
                                            (GraphPair[Parentcur->ID].vertex_List[from].vlabel == ParentDFSset->vlabelFrom)&&\
                                            (GraphPair[Parentcur->ID].vertex_List[to].vlabel == ParentDFSset->vlabelTo ))
                                        {
                                             EdgeTraveled[ GraphPair[Parentcur->ID].vertex_List[from].edge_list[i].EdgeId ]=true; 
                                        }

                                    }
                                }

                                ParentDFSset = &DFSpath[rootID];
                                Parentcur = &(ParentDFSset->Projected[parentID]);
                                rootID = ParentDFSset->rootID;
                                parentID = Parentcur->parentID;
                                
                             };

                             if(rootID==MAX_UINT_CONSTANT)
                             {
                                unsigned int to = Parentcur->EndID;
                                unsigned int from = Parentcur->StartID;

                               
                                DPMap[from] = ParentDFSset->from;
                                DPMap[to] = ParentDFSset->to;

                                {
                                    for(int i=0;i<GraphPair[Parentcur->ID].vertex_List[from].edge_list.size();i++)  
                                    { 
                                     
                                         if( GraphPair[Parentcur->ID].vertex_List[from].edge_list[i].VertexId == to )
                                         if( (GraphPair[Parentcur->ID].vertex_List[from].edge_list[i].elabel==ParentDFSset->elabel)&&\
                                            (GraphPair[Parentcur->ID].vertex_List[from].vlabel == ParentDFSset->vlabelFrom)&&\
                                            (GraphPair[Parentcur->ID].vertex_List[to].vlabel == ParentDFSset->vlabelTo ))
                                        {
                                             EdgeTraveled[ GraphPair[Parentcur->ID].vertex_List[from].edge_list[i].EdgeId ]=true; 
                                        }

                                    }
                                }

                             }

                        }      

                   // Backward Edge Extending 
                    if( DFSset->to < DFSset->from)
                    {
                        for(int i=0;i<GraphPair[cur->ID].vertex_List[cur->StartID].edge_list.size();i++)
                        { 

                          Edge * temp = &GraphPair[cur->ID].vertex_List[cur->StartID].edge_list[i];

                          if(( EdgeTraveled[temp->EdgeId]==false )&&( DPMap[temp->VertexId]!=MAX_UINT_CONSTANT)&&(DPMap[temp->VertexId]> DFSset->to))
                          {
                               PartialOrderKey key;

                               if( rightMostPath[DPMap[temp->VertexId]] == false )
                               {
                                    continue;
                               }

                               key.to =    DPMap[temp->VertexId];
                               key.from =  DFSset->from;

                               key.elabel = temp->elabel;
                               key.vlabelTo =   GraphPair[cur->ID].vertex_List[temp->VertexId].vlabel; 
                               key.vlabelFrom = GraphPair[cur->ID].vertex_List[cur->StartID].vlabel; 

                            


                              if(new_projected.find(key)!=new_projected.end())
                              {
                                    PDFS curEntry;
                                    curEntry.ID =               cur->ID;
                                    curEntry.StartID =     cur->StartID;
                                    curEntry.EndID  =     temp->VertexId;
                                    curEntry.parentID =            n;


                                    new_projected[key].push_back(curEntry);

                              }else{

                                    std::vector<PDFS> entry; 
                                    entry.resize(1);

                                    entry[0].ID =           cur->ID;
                                    entry[0].StartID = cur->StartID;
                                    entry[0].EndID = temp->VertexId;
                                    entry[0].parentID =           n;


                                    assert(key.from!= key.to);
                                    new_projected.insert(make_pair(key,entry)); 
                                  }  

                          }
                      }
                    }
            
                   if( DFSset->from < DFSset->to)
                    {

                         for(int i=0;i<GraphPair[cur->ID].vertex_List[cur->EndID].edge_list.size();i++)
                         {
                            Edge * temp = &GraphPair[cur->ID].vertex_List[cur->EndID].edge_list[i];
                            if( ( EdgeTraveled[temp->EdgeId]==false )&&( DPMap[temp->VertexId]!=MAX_UINT_CONSTANT)&&(DPMap[temp->VertexId] < DFSset->from))
                            {
                                PartialOrderKey key;


                               if( rightMostPath[DPMap[temp->VertexId]] == false )
                               {
                                    continue;
                               }


                                key.to    = DPMap[temp->VertexId];
                                key.from  = DFSset->to;

                                key.elabel =     temp->elabel;
                                key.vlabelTo =   GraphPair[cur->ID].vertex_List[temp->VertexId].vlabel; 
                                key.vlabelFrom = GraphPair[cur->ID].vertex_List[cur->EndID].vlabel; 



                               if(new_projected.find(key)!=new_projected.end())
                               {

                                    PDFS curEntry; 
                                    curEntry.ID =            cur->ID;
                                    curEntry.StartID =    cur->EndID;
                                    curEntry.EndID  = temp->VertexId;
                                    curEntry.parentID =            n;

                                    new_projected[key].push_back(curEntry);

                               }else{

                                    std::vector<PDFS> entry; 
                                    entry.resize(1);

                                    entry[0].ID =           cur->ID;
                                    entry[0].StartID =   cur->EndID;
                                    entry[0].EndID = temp->VertexId;
                                    entry[0].parentID =           n;


                                    assert(key.from!= key.to);
                                    new_projected.insert(make_pair(key,entry)); 
                                  }  
                            }                              
                         }
                    }
                //  Forward Edge Extending  
                     if( DFSset->to < DFSset->from)
                     {
                         for(int i=0;i<GraphPair[cur->ID].vertex_List.size();i++)
                         {
                            if((DPMap[i]!= MAX_UINT_CONSTANT)&&(DPMap[i] <= DFSset->from))
                            {

                                if( rightMostPath[DPMap[i]] == false )
                                {
                                        continue;   
                                }


                                for(int j=0;j<GraphPair[cur->ID].vertex_List[i].edge_list.size();j++)
                                {
                                     Edge * temp = &GraphPair[cur->ID].vertex_List[i].edge_list[j];

                                      if((EdgeTraveled[temp->EdgeId]==false)&&(DPMap[temp->VertexId]==MAX_UINT_CONSTANT))
                                      {
                                          PartialOrderKey key;



                                          key.to    = DFSset->from+1;
                                          key.from  = DPMap[i];

                                          key.elabel = temp->elabel;
                                          key.vlabelTo =   GraphPair[cur->ID].vertex_List[temp->VertexId].vlabel; 
                                          key.vlabelFrom = GraphPair[cur->ID].vertex_List[i].vlabel;  

                                      

                                         if(new_projected.find(key)!=new_projected.end())
                                        {
                                            PDFS curEntry;
                                            curEntry.ID =            cur->ID;
                                            curEntry.StartID =             i;
                                            curEntry.EndID  = temp->VertexId;
                                            curEntry.parentID =            n;
                                            new_projected[key].push_back(curEntry);

                                        }else{

                                             std::vector<PDFS> entry; 
                                             entry.resize(1);
                                             entry[0].ID =           cur->ID;
                                             entry[0].StartID =            i;
                                             entry[0].EndID = temp->VertexId;
                                             entry[0].parentID =           n;
                                             assert(key.from!= key.to);
                                             new_projected.insert(make_pair(key,entry)); 

                                            }  
                                      }
                                }
                            }
                         }
                     }
                     if( DFSset->to > DFSset->from)
                     {
                         for(int i=0;i<GraphPair[cur->ID].vertex_List.size();i++)
                         {
                            if((DPMap[i]!=MAX_UINT_CONSTANT)&&(DPMap[i] <=  DFSset->to))
                            {

                                if( rightMostPath[DPMap[i]] == false )
                                {
                                    continue;   
                                }
                                for(int j=0;j<GraphPair[cur->ID].vertex_List[i].edge_list.size();j++)
                                {
                                     Edge * temp = &GraphPair[cur->ID].vertex_List[i].edge_list[j];

                                      if((EdgeTraveled[temp->EdgeId]==false)&&(DPMap[temp->VertexId]==MAX_UINT_CONSTANT))
                                      {
                                            PartialOrderKey key;




                                            key.to    =  DFSset->to+1;
                                            key.from  =  DPMap[i];
                                            key.elabel = temp->elabel;
                                            key.vlabelTo =   GraphPair[cur->ID].vertex_List[temp->VertexId].vlabel; 
                                            key.vlabelFrom = GraphPair[cur->ID].vertex_List[i].vlabel;   


                                         if(new_projected.find(key)!=new_projected.end())
                                        {
                                            PDFS curEntry;
                                            curEntry.ID =            cur->ID;
                                            curEntry.StartID =             i;
                                            curEntry.EndID  = temp->VertexId;
                                            curEntry.parentID =            n;
                                            new_projected[key].push_back(curEntry);

                                        }else{

                                             std::vector<PDFS> entry; 
                                             entry.resize(1);
                                             entry[0].ID =           cur->ID;
                                             entry[0].StartID =            i;
                                             entry[0].EndID = temp->VertexId;
                                             entry[0].parentID =           n;
                                             assert(key.from!= key.to);
                                             new_projected.insert(make_pair(key,entry)); 
                                            }  
                                      }
                                }
                            }
                         }
                    }
               }


             unsigned int ancestorID; 
             ancestorID = DFSpath.size()-1;
             bool isReported = true; 
             unsigned int Population;




            for(Projected_map::iterator cur = new_projected.begin();cur != new_projected.end(); ++cur){

                   unsigned int Population;
                   Support(Population,cur->second);
                   DFSset=&DFSpath[ancestorID];

                   if( Population == DFSset->support )
                   {
                      isReported = false; 
                   } 
                   if(Population>1){
                      DFSpath.push(Population,cur->first.elabel,cur->first.vlabelFrom,cur->first.vlabelTo,cur->second,ancestorID,cur->first.from,cur->first.to);
                      break;
                   }
            }


            if(isReported)
            {

                 DFS * ParentDFSset= &DFSpath[ancestorID];


                 unsigned int length = std::max(ParentDFSset->from,ParentDFSset->to)+1;
                 unsigned int depth = 1;


                 while(ParentDFSset->rootID!= MAX_UINT_CONSTANT)
                 {
                    ParentDFSset = &DFSpath[ParentDFSset->rootID];
                    depth=depth+1;
                 }






                if((depth < (g1.EdgeSize))&&(depth< (g2.EdgeSize)))
                {
                     return false; 
                }

                {

                     {
                        ParentDFSset= &DFSpath[ancestorID];
                        MatchedGraphsIDCountArray.resize(0);
                        MatchedGraphsIDArray.resize(0);

                        unsigned int oldID = (ParentDFSset->Projected.begin())->ID;
                        unsigned int Count =0;

                        for (std::vector<PDFS>::iterator cur = ParentDFSset->Projected.begin(); cur != ParentDFSset->Projected.end(); ++cur) {
                            if (oldID != cur->ID) {
                             CountG1 = Count;
                             Count =0;
                            }
                        Count=Count+1; 
                        oldID=cur->ID;
                        }
                        CountG2 = Count;

                      }

                }


                if(isTrace == true)
                {

                        TraceG1.resize(0);
                        TraceG2.resize(0);


                        for(int i=0;i< DFSpath[ancestorID].Projected.size();i++)
                        {

                            DFS * ParentDFSset= &DFSpath[ancestorID];
                            unsigned int to =    (*ParentDFSset).to;
                            unsigned int from =  (*ParentDFSset).from; 
                            std::vector<unsigned int> * TraceEntry;

                            if((*ParentDFSset).Projected[i].ID==0)
                            {
                                TraceG1.resize(TraceG1.size()+1);
                                TraceEntry = &TraceG1[TraceG1.size()-1];

                            }else if((*ParentDFSset).Projected[i].ID==1)
                            {
                                TraceG2.resize(TraceG2.size()+1);
                                TraceEntry = &TraceG2[TraceG2.size()-1];

                            }else{ assert(0);}

                            TraceEntry->resize(length);


                            unsigned int rootID = ParentDFSset->rootID;
                            unsigned int parentID = i;


                             while(rootID!=MAX_UINT_CONSTANT)
                             {
                                (*TraceEntry)[ParentDFSset->from] = (*ParentDFSset).Projected[parentID].StartID;
                                (*TraceEntry)[ParentDFSset->to] = (*ParentDFSset).Projected[parentID].EndID;
                                parentID =(*ParentDFSset).Projected[parentID].parentID;
                                ParentDFSset = &DFSpath[rootID];
                                rootID = ParentDFSset->rootID;
                             }

                             if(ParentDFSset->rootID == MAX_UINT_CONSTANT)
                             {
                                (*TraceEntry)[ParentDFSset->from] = (*ParentDFSset).Projected[parentID].StartID;
                                (*TraceEntry)[ParentDFSset->to] = (*ParentDFSset).Projected[parentID].EndID;
                             } 

                        }
                }
                return true;  
            }
    }

      return false; 
}






void ComStruct::RemoveDuplicates() 
{   


      bool flag = isTrace;
      isTrace = false;

      std::vector<size_t> idx(FrequentGraphsIDArray.size());
      std::iota(idx.begin(), idx.end(), 0);
      sort(idx.begin(), idx.end(),[&](size_t i1, size_t i2) { if(FrequentGraphsIDArray[i1]!=FrequentGraphsIDArray[i2]) \
        {return FrequentGraphsIDArray[i1] < FrequentGraphsIDArray[i2]; } \
        else{return (FrequentGraphs[i1].EdgeSize  > FrequentGraphs[i2].EdgeSize);}});

      for(unsigned int i1=0;i1<idx.size();)
      {
          unsigned int i2;

           for(i2=i1+1;i2<idx.size();i2++)
          {
              if(FrequentGraphsIDArray[idx[i1]]!=FrequentGraphsIDArray[idx[i2]])
              {
                    break;
              }
          }  

        for(int k1 =i1;k1<i2;k1++)
        {

            unsigned int IsomorphismG1_G1;
            unsigned int IsomorphismG1_G2;



           //   if(IsExtract==true)
           // {
           //     if( FrequentGraphsIDArray[idx[k1]].size()!=0 )
           //     graphMatch(FrequentGraphs[idx[k1]],FrequentGraphs[idx[k1]]);
           //     IsomorphismG1_G1 = CountG1;
           // }


            for(int k2 = k1+1;k2<i2;k2++)
            {
                 if((FrequentGraphsIDArray[idx[k1]]!=0)&&(FrequentGraphsIDArray[idx[k2]]!=0))
                 {
                         if(IsConteined(FrequentGraphs[idx[k1]],FrequentGraphs[idx[k2]]))
                         {
                            {
                               FrequentGraphsIDArray[idx[k2]]=0;
                            }
                         }
                 }

            }
         }
            i1 = i2; 

      }
      isTrace = flag;
}


 UniqueGraphSet::UniqueGraphSet()
{
    Match.isTrace = false;
}



bool UniqueGraphSet::MPinsert(Graph_& g)
{
    MPINDEX::iterator it;

      it = GraphIndex.find(g);

      if(it!=GraphIndex.end())
      {
       
          for(int i=0;i<it->second.size();i++)
          {
             if( Match.graphMatch(g,GraphSet[it->second[i]]))
             {
                return false;
             }           
          }

          it->second.resize(it->second.size()+1);
          GraphSet.push_back(g);
          it->second[it->second.size()-1] = GraphSet.size()-1;
          
          return true;


      }else{
          std::vector<unsigned int>entry;
          entry.resize(1);
          GraphSet.push_back(g);
          entry[0] = GraphSet.size()-1;
          GraphIndex.insert(make_pair(g,entry));
          return true; 
      }

          return false;

}




unsigned int  UniqueGraphSet::MPfind(Graph_& g)
{ 

      MPINDEX::iterator it;

      it = GraphIndex.find(g);
      if(it!=GraphIndex.end())
      {
       
          for(int i=0;i<it->second.size();i++)
          {
             if( Match.graphMatch(g,GraphSet[it->second[i]]))
             {
                return it->second[i];
             }           
          }
      }

      return MAX_UINT_CONSTANT;
} 




bool ComStruct::graphBulkMatch(Graph_ &g1)
{

       DFSpath.clear();
       new_projected.clear();


       if(isTrace==true)
       {
         TraceArray.resize(0);
         TraceSingle.resize(0);
       }


       
       Sample.resize(Sample.size()+1); 


       Graph_ * NewEntry = &Sample[Sample.size()-1]; 

       (*NewEntry).vertex_List.resize(g1.vertex_List.size());


       unsigned int KeyID; 

       KeyID = Sample.size()-1; 


       for(int i1=0;i1<g1.vertex_List.size();i1++)
       {
           (*NewEntry).vertex_List[i1].vlabel  = g1.vertex_List[i1].vlabel;
           (*NewEntry).vertex_List[i1].edge_list.resize(0);
       }

       for(int i =0;i<g1.EdgeSize;i++)
       {
            CompareContext1[i]=0;
       }




        for(int i1=0;i1<g1.vertex_List.size();i1++)
        {

               for( int j1 =0;j1<g1.vertex_List[i1].edge_list.size();j1++)
              {
                  unsigned int EdgeID; 

                  EdgeID = g1.vertex_List[i1].edge_list[j1].EdgeId;
                  if(CompareContext1[EdgeID]!=MAX_UINT_CONSTANT)
                  {
                    unsigned int id1 =  i1;
                    unsigned int id2 =  g1.vertex_List[i1].edge_list[j1].VertexId;
       

                    (*NewEntry).vertex_List[id1].edge_list.resize((*NewEntry).vertex_List[id1].edge_list.size()+1);
                    (*NewEntry).vertex_List[id1].edge_list[(*NewEntry).vertex_List[id1].edge_list.size()-1].VertexId = id2;
                    (*NewEntry).vertex_List[id1].edge_list[(*NewEntry).vertex_List[id1].edge_list.size()-1].elabel  = g1.vertex_List[i1].edge_list[j1].elabel;
                    (*NewEntry).vertex_List[id1].edge_list[(*NewEntry).vertex_List[id1].edge_list.size()-1].EdgeId  = g1.vertex_List[i1].edge_list[j1].EdgeId;


                    (*NewEntry).vertex_List[id2].edge_list.resize((*NewEntry).vertex_List[id2].edge_list.size()+1);
                    (*NewEntry).vertex_List[id2].edge_list[(*NewEntry).vertex_List[id2].edge_list.size()-1].VertexId = id1;
                    (*NewEntry).vertex_List[id2].edge_list[(*NewEntry).vertex_List[id2].edge_list.size()-1].elabel  = g1.vertex_List[i1].edge_list[j1].elabel;
                    (*NewEntry).vertex_List[id2].edge_list[(*NewEntry).vertex_List[id2].edge_list.size()-1].EdgeId =  g1.vertex_List[i1].edge_list[j1].EdgeId;


                    CompareContext1[EdgeID] = MAX_UINT_CONSTANT;
                  }
             
              }
        }


        (*NewEntry).EdgeSize = g1.EdgeSize;
       


     if(isSelected)
     {
                SubSet.resize(SubSet.size()+1);
                SubSet[SubSet.size()-1] = KeyID; 

                for (int i = 0;i<SubSet.size();i++) 
                { 
                    for(int j=0;j<Sample[SubSet[i]].vertex_List.size();j++)
                    {

                        PDFS cur;
                        cur.ID = SubSet[i]; 

                       for(int j2=0;j2< Sample[SubSet[i]].vertex_List[j].edge_list.size();j2++) 
                       {
                        // if( GraphPair[i].vertex_List[j].VertexId<GraphPair[i].vertex_List[j].edge_list[j2].VertexId)
                        // {
                            cur.StartID = j;
                            cur.EndID   = Sample[SubSet[i]].vertex_List[j].edge_list[j2].VertexId; 

                            PartialOrderKey key;

                            
                            key.from       = 0;
                            key.to         = 1;
                            key.vlabelFrom = Sample[SubSet[i]].vertex_List[cur.StartID].vlabel;
                            key.vlabelTo   = Sample[SubSet[i]].vertex_List[cur.EndID].vlabel;
                            key.elabel     = Sample[SubSet[i]].vertex_List[cur.StartID].edge_list[j2].elabel;


                           if(new_projected.find(key)!=new_projected.end())
                          {
                               new_projected[key].push_back(cur);

                          }else{

                              std::vector<PDFS> entry; 
                              entry.push_back(cur);
                              assert(key.from!= key.to);
                              new_projected.insert(make_pair(key,entry)); 
                           }  
                        }
                    }
             }  


     }else{


                for (int i = 0;i<Sample.size();i++) 
                { 
                    for(int j=0;j<Sample[i].vertex_List.size();j++)
                    {

                        PDFS cur;
                        cur.ID = i; 

                       for(int j2=0;j2< Sample[i].vertex_List[j].edge_list.size();j2++) 
                       {
                        // if( GraphPair[i].vertex_List[j].VertexId<GraphPair[i].vertex_List[j].edge_list[j2].VertexId)
                        // {
                            cur.StartID = j;
                            cur.EndID   = Sample[i].vertex_List[j].edge_list[j2].VertexId; 

                            PartialOrderKey key;

                            
                            key.from       = 0;
                            key.to         = 1;
                            key.vlabelFrom = Sample[i].vertex_List[cur.StartID].vlabel;
                            key.vlabelTo   = Sample[i].vertex_List[cur.EndID].vlabel;
                            key.elabel     = Sample[i].vertex_List[cur.StartID].edge_list[j2].elabel;


                           if(new_projected.find(key)!=new_projected.end())
                          {
                               new_projected[key].push_back(cur);

                          }else{

                              std::vector<PDFS> entry; 
                              entry.push_back(cur);
                              assert(key.from!= key.to);
                              new_projected.insert(make_pair(key,entry)); 
                           }  
                        }
                    }
             }  

     }




  

    for(Projected_map::iterator cur = new_projected.begin();cur != new_projected.end(); ++cur){
       
       if((*(cur->second).rbegin()).ID == KeyID)
        {
            DFSpath.push(1.0,cur->first.elabel,cur->first.vlabelFrom,cur->first.vlabelTo,cur->second,MAX_UINT_CONSTANT,cur->first.from,cur->first.to);
        }
    }


     while(DFSpath.size())
     { 



            std::vector<bool>          rightMostPath;
            std::vector<rootComposeID>   rootIDArray;
         



                    DFS * DFSset = &DFSpath[DFSpath.size()-1];
                   

                    if( DFSset->isTraveled == false )
                     {
                         DFSset->isTraveled = true;
                     }else
                     {
                         DFSpath.pop();
                         continue;
                     }



                    new_projected.clear();
                    rightMostPath.resize(std::max(DFSset->from,DFSset->to)+1);
                    std::fill(rightMostPath.begin(),rightMostPath.end(),false);
                    rightMostPath[std::max(DFSset->from,DFSset->to)] = true;
                    unsigned int OldRmpV = std::max(DFSset->from,DFSset->to);
                    unsigned int rootID;
                

                    DFS * ParentDFSset = DFSset;
                    rootID = ParentDFSset->rootID;

                        while((rootID!=MAX_UINT_CONSTANT))
                        {
                    

                             if( ParentDFSset->from < ParentDFSset->to)
                             {
                                 if(ParentDFSset->to==OldRmpV)
                                 {
                                    OldRmpV =ParentDFSset->from;
                                 }
                                rightMostPath[OldRmpV] = true;
                             }
                        
                          
                            ParentDFSset = &DFSpath[rootID];
                            rootID = ParentDFSset->rootID;

                        };//To find rightmost path. 
                        if(rootID==MAX_UINT_CONSTANT)
                        {
                                if( ParentDFSset->from < ParentDFSset->to)
                                {
                                    if(ParentDFSset->to==OldRmpV)
                                    {
                                        OldRmpV =ParentDFSset->from;
                                    }
                                        rightMostPath[OldRmpV] = true;
                                }
                        }


                




                for (unsigned int n = 0; n < DFSset->Projected.size(); ++n)
                {
                    PDFS *cur = &DFSset->Projected[n];

                    unsigned int rootID   =   DFSset->rootID;
                    unsigned int parentID =   cur->parentID;
                    std::vector<bool> EdgeTraveled;
                    EdgeTraveled.resize(Sample[cur->ID].EdgeSize); 
                    std::fill(EdgeTraveled.begin(), EdgeTraveled.end(),false);
                    std::vector< unsigned int > DPMap;
                    DPMap.resize(Sample[cur->ID].vertex_List.size()+1);
                    std::fill(DPMap.begin(),DPMap.end(),MAX_UINT_CONSTANT);
               


                    DPMap[cur->StartID] = DFSset->from;
                    DPMap[cur->EndID] =  DFSset->to;



                    DFS * ParentDFSset = NULL;
                    PDFS * Parentcur = NULL;
                    {


                            Parentcur =cur;
                            ParentDFSset=DFSset;
                            rootID = ParentDFSset->rootID;
                            parentID = Parentcur->parentID;


                            while(rootID!=MAX_UINT_CONSTANT)
                            {
                            
                                unsigned int to = Parentcur->EndID;
                                unsigned int from = Parentcur->StartID;

                               
                                DPMap[from] = ParentDFSset->from;
                                DPMap[to] = ParentDFSset->to;

                                {
                                    for(int i=0;i<Sample[Parentcur->ID].vertex_List[from].edge_list.size();i++)  
                                    { 
                                     
                                         if( Sample[Parentcur->ID].vertex_List[from].edge_list[i].VertexId == to )
                                         if( (Sample[Parentcur->ID].vertex_List[from].edge_list[i].elabel==ParentDFSset->elabel)&&\
                                            (Sample[Parentcur->ID].vertex_List[from].vlabel == ParentDFSset->vlabelFrom)&&\
                                            (Sample[Parentcur->ID].vertex_List[to].vlabel == ParentDFSset->vlabelTo ))
                                        {
                                             EdgeTraveled[ Sample[Parentcur->ID].vertex_List[from].edge_list[i].EdgeId ]=true; 
                                        }

                                    }
                                }

                                ParentDFSset = &DFSpath[rootID];
                                Parentcur = &(ParentDFSset->Projected[parentID]);
                                rootID = ParentDFSset->rootID;
                                parentID = Parentcur->parentID;
                                
                             };

                             if(rootID==MAX_UINT_CONSTANT)
                             {
                                unsigned int to = Parentcur->EndID;
                                unsigned int from = Parentcur->StartID;

                               
                                DPMap[from] = ParentDFSset->from;
                                DPMap[to] = ParentDFSset->to;

                                {
                                    for(int i=0;i<Sample[Parentcur->ID].vertex_List[from].edge_list.size();i++)  
                                    { 
                                     
                                         if( Sample[Parentcur->ID].vertex_List[from].edge_list[i].VertexId == to )
                                         if( (Sample[Parentcur->ID].vertex_List[from].edge_list[i].elabel==ParentDFSset->elabel)&&\
                                            (Sample[Parentcur->ID].vertex_List[from].vlabel == ParentDFSset->vlabelFrom)&&\
                                            (Sample[Parentcur->ID].vertex_List[to].vlabel == ParentDFSset->vlabelTo ))
                                        {
                                             EdgeTraveled[ Sample[Parentcur->ID].vertex_List[from].edge_list[i].EdgeId ]=true; 
                                        }

                                    }
                                }

                             }

                        }      

                   // Backward Edge Extending 
                    if( DFSset->to < DFSset->from)
                    {
                        for(int i=0;i<Sample[cur->ID].vertex_List[cur->StartID].edge_list.size();i++)
                        { 

                          Edge * temp = &Sample[cur->ID].vertex_List[cur->StartID].edge_list[i];

                          if(( EdgeTraveled[temp->EdgeId]==false )&&( DPMap[temp->VertexId]!=MAX_UINT_CONSTANT)&&(DPMap[temp->VertexId] >= DFSset->to))
                          {
                               PartialOrderKey key;

                               if( rightMostPath[DPMap[temp->VertexId]] == false )
                               {
                                    continue;
                               }

                               key.to =    DPMap[temp->VertexId];
                               key.from =  DFSset->from;

                               key.elabel = temp->elabel;
                               key.vlabelTo =   Sample[cur->ID].vertex_List[temp->VertexId].vlabel; 
                               key.vlabelFrom = Sample[cur->ID].vertex_List[cur->StartID].vlabel; 

                            


                              if(new_projected.find(key)!=new_projected.end())
                              {
                                    PDFS curEntry;
                                    curEntry.ID =               cur->ID;
                                    curEntry.StartID =     cur->StartID;
                                    curEntry.EndID  =     temp->VertexId;
                                    curEntry.parentID =            n;


                                    new_projected[key].push_back(curEntry);

                              }else{

                                    std::vector<PDFS> entry; 
                                    entry.resize(1);

                                    entry[0].ID =           cur->ID;
                                    entry[0].StartID = cur->StartID;
                                    entry[0].EndID = temp->VertexId;
                                    entry[0].parentID =           n;


                                    assert(key.from!= key.to);
                                    new_projected.insert(make_pair(key,entry)); 
                                  }  

                          }
                      }
                    }
            
                   if( DFSset->from < DFSset->to)
                    {

                         for(int i=0;i<Sample[cur->ID].vertex_List[cur->EndID].edge_list.size();i++)
                         {
                            Edge * temp = &Sample[cur->ID].vertex_List[cur->EndID].edge_list[i];
                            if( ( EdgeTraveled[temp->EdgeId]==false )&&( DPMap[temp->VertexId]!=MAX_UINT_CONSTANT)&&(DPMap[temp->VertexId] <= DFSset->from))
                            {
                                PartialOrderKey key;


                               if( rightMostPath[DPMap[temp->VertexId]] == false )
                               {
                                    continue;
                               }


                                key.to    = DPMap[temp->VertexId];
                                key.from  = DFSset->to;

                                key.elabel =     temp->elabel;
                                key.vlabelTo =   Sample[cur->ID].vertex_List[temp->VertexId].vlabel; 
                                key.vlabelFrom = Sample[cur->ID].vertex_List[cur->EndID].vlabel; 



                               if(new_projected.find(key)!=new_projected.end())
                               {

                                    PDFS curEntry; 
                                    curEntry.ID =            cur->ID;
                                    curEntry.StartID =    cur->EndID;
                                    curEntry.EndID  = temp->VertexId;
                                    curEntry.parentID =            n;

                                    new_projected[key].push_back(curEntry);

                               }else{

                                    std::vector<PDFS> entry; 
                                    entry.resize(1);

                                    entry[0].ID =           cur->ID;
                                    entry[0].StartID =   cur->EndID;
                                    entry[0].EndID = temp->VertexId;
                                    entry[0].parentID =           n;


                                    assert(key.from!= key.to);
                                    new_projected.insert(make_pair(key,entry)); 
                                  }  
                            }                              
                         }
                    }
                //  Forward Edge Extending  
                     if( DFSset->to < DFSset->from)
                     {
                         for(int i=0;i<Sample[cur->ID].vertex_List.size();i++)
                         {
                            if((DPMap[i]!= MAX_UINT_CONSTANT)&&(DPMap[i] <= DFSset->from))
                            {

                                if( rightMostPath[DPMap[i]] == false )
                                {
                                        continue;   
                                }


                                for(int j=0;j<Sample[cur->ID].vertex_List[i].edge_list.size();j++)
                                {
                                     Edge * temp = &Sample[cur->ID].vertex_List[i].edge_list[j];

                                      if((EdgeTraveled[temp->EdgeId]==false)&&(DPMap[temp->VertexId]==MAX_UINT_CONSTANT))
                                      {
                                          PartialOrderKey key;



                                          key.to    = DFSset->from+1;
                                          key.from  = DPMap[i];

                                          key.elabel = temp->elabel;
                                          key.vlabelTo =   Sample[cur->ID].vertex_List[temp->VertexId].vlabel; 
                                          key.vlabelFrom = Sample[cur->ID].vertex_List[i].vlabel;  

                                      

                                         if(new_projected.find(key)!=new_projected.end())
                                        {
                                            PDFS curEntry;
                                            curEntry.ID =            cur->ID;
                                            curEntry.StartID =             i;
                                            curEntry.EndID  = temp->VertexId;
                                            curEntry.parentID =            n;
                                            new_projected[key].push_back(curEntry);

                                        }else{

                                             std::vector<PDFS> entry; 
                                             entry.resize(1);
                                             entry[0].ID =           cur->ID;
                                             entry[0].StartID =            i;
                                             entry[0].EndID = temp->VertexId;
                                             entry[0].parentID =           n;
                                             assert(key.from!= key.to);
                                             new_projected.insert(make_pair(key,entry)); 

                                            }  
                                      }
                                }
                            }
                         }
                     }
                     if( DFSset->to > DFSset->from)
                     {
                         for(int i=0;i<Sample[cur->ID].vertex_List.size();i++)
                         {
                            if((DPMap[i]!=MAX_UINT_CONSTANT)&&(DPMap[i] <=  DFSset->to))
                            {

                                if( rightMostPath[DPMap[i]] == false )
                                {
                                    continue;   
                                }
                                for(int j=0;j<Sample[cur->ID].vertex_List[i].edge_list.size();j++)
                                {
                                     Edge * temp = &Sample[cur->ID].vertex_List[i].edge_list[j];

                                      if((EdgeTraveled[temp->EdgeId]==false)&&(DPMap[temp->VertexId]==MAX_UINT_CONSTANT))
                                      {
                                            PartialOrderKey key;




                                            key.to    =  DFSset->to+1;
                                            key.from  =  DPMap[i];
                                            key.elabel = temp->elabel;
                                            key.vlabelTo =   Sample[cur->ID].vertex_List[temp->VertexId].vlabel; 
                                            key.vlabelFrom = Sample[cur->ID].vertex_List[i].vlabel;   


                                         if(new_projected.find(key)!=new_projected.end())
                                        {
                                            PDFS curEntry;
                                            curEntry.ID =            cur->ID;
                                            curEntry.StartID =             i;
                                            curEntry.EndID  = temp->VertexId;
                                            curEntry.parentID =            n;
                                            new_projected[key].push_back(curEntry);

                                        }else{

                                             std::vector<PDFS> entry; 
                                             entry.resize(1);
                                             entry[0].ID =           cur->ID;
                                             entry[0].StartID =            i;
                                             entry[0].EndID = temp->VertexId;
                                             entry[0].parentID =           n;
                                             assert(key.from!= key.to);
                                             new_projected.insert(make_pair(key,entry)); 
                                            }  
                                      }
                                }
                            }
                         }
                    }
               }


             unsigned int ancestorID; 
             ancestorID = DFSpath.size()-1;
             bool isReported = true; 
             unsigned int Population;




            for(Projected_map::iterator cur = new_projected.begin();cur != new_projected.end(); ++cur){

                   unsigned int Population;
                   DFSset=&DFSpath[ancestorID];


                   if( (*(cur->second).rbegin()).ID == KeyID)
                   {    
                        Population =1;
                   }else
                   {
                        Population =0;
                   }
                   if(DFSset->support == Population)
                   {
                       isReported =false; 
                   }
               
                   if(Population==1) {
                      DFSpath.push(Population,cur->first.elabel,cur->first.vlabelFrom,cur->first.vlabelTo,cur->second,ancestorID,cur->first.from,cur->first.to);
                      break;
                   }
            }


            if(isReported)
            {

                 DFS * ParentDFSset= &DFSpath[ancestorID];


                 unsigned int length = std::max(ParentDFSset->from,ParentDFSset->to)+1;
                 unsigned int depth = 1;


                 while(ParentDFSset->rootID!= MAX_UINT_CONSTANT)
                 {
                    ParentDFSset = &DFSpath[ParentDFSset->rootID];
                    depth=depth+1;
                 }

                if((depth < (g1.EdgeSize)))
                {
                     Sample.resize(Sample.size()-1);
                     if(isSelected)
                       SubSet.resize(SubSet.size()-1);
                     return false; 
                }

                {
                     ParentDFSset= &DFSpath[ancestorID];
                     MatchedGraphsIDCountArray.resize(0);
                     MatchedGraphsIDArray.resize(0);

                    unsigned int oldID = (ParentDFSset->Projected.begin())->ID;

                    unsigned int Count =0;

                    for (std::vector<PDFS>::iterator cur = ParentDFSset->Projected.begin(); cur != ParentDFSset->Projected.end(); ++cur) {
                        if (oldID != cur->ID) {

                             MatchedGraphsIDArray.resize(MatchedGraphsIDArray.size()+1);
                             MatchedGraphsIDArray[MatchedGraphsIDArray.size()-1]=oldID;

                             MatchedGraphsIDCountArray.resize(MatchedGraphsIDCountArray.size()+1);
                             MatchedGraphsIDCountArray[MatchedGraphsIDCountArray.size()-1]=Count;
                             Count =0;

                            }
                        Count=Count+1; 
                        oldID=cur->ID;
                    }


                 MatchedSingleGraphCount =Count;
                 assert(oldID==KeyID);


                 if(MatchedGraphsIDArray.size() == 0)
                 {

                    Sample.resize(Sample.size()-1);
                      if(isSelected)
                    SubSet.resize(SubSet.size()-1);

                      return false;
                 }






                if(isTrace == true)
                {

                        TraceSingle.resize(0);
                        TraceArray.resize(1);
                       
                        ParentDFSset= &DFSpath[ancestorID];
                        unsigned int oldID = (ParentDFSset->Projected.begin())->ID;

                        for(int i=0;i<(DFSpath[ancestorID]).Projected.size();i++)
                        {

                            ParentDFSset= &DFSpath[ancestorID];
                            unsigned int to =    (*ParentDFSset).to;
                            unsigned int from =  (*ParentDFSset).from; 
                            std::vector<unsigned int> * TraceEntry;

                            if((*ParentDFSset).Projected[i].ID==KeyID)
                            {
                                TraceSingle.resize(TraceSingle.size()+1);
                                TraceEntry = &TraceSingle[TraceSingle.size()-1];
                            }else
                            {
                                if(oldID != (*ParentDFSset).Projected[i].ID)
                                {
                                    TraceArray.resize(TraceArray.size()+1);
                                }
                                
                                oldID = (*ParentDFSset).Projected[i].ID;

                                unsigned int ID = TraceArray.size()-1; 
                                TraceArray[ID].resize(TraceArray[ID].size()+1);
                                TraceEntry = &TraceArray[ID][TraceArray[ID].size()-1];

                            }


                            TraceEntry->resize(length);


                            unsigned int rootID = ParentDFSset->rootID;
                            unsigned int parentID = i;


                             while(rootID!=MAX_UINT_CONSTANT)
                             {
                                (*TraceEntry)[ParentDFSset->from] = (*ParentDFSset).Projected[parentID].StartID;
                                (*TraceEntry)[ParentDFSset->to] = (*ParentDFSset).Projected[parentID].EndID;
                                parentID =(*ParentDFSset).Projected[parentID].parentID;
                                ParentDFSset = &DFSpath[rootID];
                                rootID = ParentDFSset->rootID;
                             }

                             if(ParentDFSset->rootID == MAX_UINT_CONSTANT)
                             {
                                (*TraceEntry)[ParentDFSset->from] = (*ParentDFSset).Projected[parentID].StartID;
                                (*TraceEntry)[ParentDFSset->to] = (*ParentDFSset).Projected[parentID].EndID;
                             } 

                        }

                     assert(MatchedGraphsIDArray.size()==TraceArray.size());
                }


             }









               
                Sample.resize(Sample.size()-1);
                if(isSelected)
                SubSet.resize(SubSet.size()-1);
                return true;  
            }
    }

      Sample.resize(Sample.size()-1);
      if(isSelected)
      SubSet.resize(SubSet.size()-1);
      return false; 
}


bool ComStruct::Is_minimum( unsigned int ID_,unsigned int to,unsigned int from,uint16_t vlabelFrom,uint16_t vlabelTo,uint32_t elabel,bool isTraced)
{
    unsigned int index;
    unsigned int maxNode; 
    unsigned ID = ID_;

    index = 0;
    minimumGraphContext[index].to = to;
    minimumGraphContext[index].from =from;
    minimumGraphContext[index].vlabelFrom = vlabelFrom;
    minimumGraphContext[index].vlabelTo = vlabelTo;
    minimumGraphContext[index].elabel =elabel;
    ID = ID_; 
   
    while( ID != MAX_UINT_CONSTANT)
    {

        index = index +1;
        minimumGraphContext[index].to = DFSpath[ID].to;
        minimumGraphContext[index].from = DFSpath[ID].from;
        minimumGraphContext[index].vlabelFrom = DFSpath[ID].vlabelFrom;
        minimumGraphContext[index].vlabelTo = DFSpath[ID].vlabelTo;
        minimumGraphContext[index].elabel = DFSpath[ID].elabel;
        minimumGraphTraveled[index] = false;
        ID = DFSpath[ID].rootID;
    }


    for(int i=0;i<(index-i);i++)
    {
        PartialOrderKey Temp;
        Temp = minimumGraphContext[i]; 
        minimumGraphContext[i] = minimumGraphContext[index-i];
        minimumGraphContext[index-i] = Temp;
    }
   //reverse the order
    
   if(isTraced==false)
   { 
      maxNode = std::max(minimumGraphContext[index].to,minimumGraphContext[index].from);
      //Note that!: the DFS code to check must be the rightmost extension code. 

   }else
   { 
      maxNode = std::max(minimumGraphContext[index-1].to,minimumGraphContext[index-1].from);
      maxNode = std::max(maxNode,minimumGraphContext[index].from);
      maxNode = std::max(maxNode,minimumGraphContext[index].to);


     //Note that! in the traced model, the (index-1)th prefix string of the DFS code to be checked must be the rightmost extension code.

   }



 {

        PartialOrderKeyThin e1,e2;
        PartialOrderKeyThinCompare Compare;
        std::queue< unsigned int >  EqualList; 
        PartialOrderKeyThin min;
        int  minID=0;
        bool isMatched = false;
        unsigned int MatchedID = 0;
        std::vector<size_t> idx((index+1)*2);
        std::vector<size_t>  EqualTransitive((index+1)*2); 
        std::iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(),[&](size_t i1, size_t i2) {if(i1>index)  \
            { e1.vlabelTo = minimumGraphContext[i1-index-1].vlabelFrom;e1.vlabelFrom = minimumGraphContext[i1-index-1].vlabelTo;e1.elabel= minimumGraphContext[i1-index-1].elabel; } \
            else { \
                e1.vlabelTo = minimumGraphContext[i1].vlabelTo;e1.vlabelFrom = minimumGraphContext[i1].vlabelFrom; e1.elabel = minimumGraphContext[i1].elabel; } \
                if(i2>index){ e2.vlabelTo = minimumGraphContext[i2-index-1].vlabelFrom;e2.vlabelFrom = minimumGraphContext[i2-index-1].vlabelTo;e2.elabel= minimumGraphContext[i2-index-1].elabel; } \
                else { e2.vlabelTo = minimumGraphContext[i2].vlabelTo;e2.vlabelFrom = minimumGraphContext[i2].vlabelFrom; e2.elabel = minimumGraphContext[i2].elabel;  } \
                return (Compare(e1,e2));});


       {
                PartialOrderKeyThin a;

                   a.elabel  =  0;
                   a.vlabelTo  = 0;
                   a.vlabelFrom = 0;
                   unsigned int  Accumulation = 0; 


                    for(int i=0;i<(((index+1)*2));i++)
                   {
                   
                        PartialOrderKeyThin b;

                                 if(idx[i]>index)
                                 {   
                                     b.elabel  =  minimumGraphContext[idx[i]-index-1].elabel;
                                     b.vlabelFrom = minimumGraphContext[idx[i]-index-1].vlabelTo;
                                     b.vlabelTo = minimumGraphContext[idx[i]-index-1].vlabelFrom;   
                                 }else
                                 { 
                                     b.elabel  =  minimumGraphContext[idx[i]].elabel;
                                     b.vlabelTo  = minimumGraphContext[idx[i]].vlabelTo;
                                     b.vlabelFrom = minimumGraphContext[idx[i]].vlabelFrom;
                                 }

                               if (!((a.vlabelTo == b.vlabelTo)&&(a.vlabelFrom == b.vlabelFrom)&&(a.elabel==b.elabel)))
                               {
                                    Accumulation = Accumulation +1;
                                    a = b;
                               }
                                    EqualTransitive[i]=Accumulation; 

                    }

       }



  
          unsigned int EqualID = EqualTransitive[minID]; 
          unsigned int last_ID=((index+1)*2)-1;
          unsigned int stackSize = 0;

          minimumGraphStack[stackSize].to =   1;
          minimumGraphStack[stackSize].from = 0;
          minimumGraphStack[stackSize].ancestor = MAX_UINT_CONSTANT16;
          minimumGraphStack[stackSize].begin = 0;
          minimumGraphStack[stackSize].end = 0; 
          minimumGraphStack[stackSize].rank = EqualTransitive[minID];
  


                    for(int i = minID;i<(((index+1)*2));i++)
                    {
                        if(EqualTransitive[i] == EqualID)
                        {
                          
                            minimumGraphStackList[ minimumGraphStack[stackSize].end ].ID = i; 
                            minimumGraphStackList[ minimumGraphStack[stackSize].end ].parentID = MAX_UINT_CONSTANT;
                            minimumGraphStack[stackSize].end = minimumGraphStack[stackSize].end+1;

                            if(isTraced==false)
                            {
                               if(( idx[i] == MatchedID ))
                               {  
                                  isMatched = true;
                               }
                            }

                        }

                    }


                     if(isTraced==false)
                     {
                            if(isMatched == false)
                           {
                              return false;

                           }else
                           {     
                              isMatched = false;
                              MatchedID = MatchedID+1;
                           }
                     }



                     while(stackSize<index)
                     {
                        unsigned int ancestorID = stackSize; 
                        stackSize = stackSize+1;

                        minimumGraphStack[stackSize].to   = MAX_UINT_CONSTANT16;
                        minimumGraphStack[stackSize].from = 0;
                        minimumGraphStack[stackSize].rank = MAX_UINT_CONSTANT16;
                        minimumGraphStack[stackSize].ancestor = ancestorID;
                        minimumGraphStack[stackSize].begin = minimumGraphStack[ancestorID].end;
                        minimumGraphStack[stackSize].end = minimumGraphStack[stackSize].begin;




                        for(int i = minimumGraphStack[ancestorID].begin; i< minimumGraphStack[ancestorID].end;i++)
                        {
                            unsigned int  currentID   =  ancestorID;
                            unsigned int  parentID =  i;
                            unsigned int  ID;



                            for(int i2 = 0;i2<=maxNode;i2++)
                            {
                                node_mapping[i2] = MAX_UINT_CONSTANT16;
                            }

                            for(int i3 = 0;i3 <= index;i3++)
                            {
                                minimumGraphTraveled[i3] = false;
                            }

                            while( parentID!=MAX_UINT_CONSTANT)
                            {
                                ID  = minimumGraphStackList[parentID].ID;


                                if(idx[ID]>index)
                                {   

                                    node_mapping[minimumGraphContext[idx[ID]-index-1].to]   = minimumGraphStack[currentID].from;
                                    node_mapping[minimumGraphContext[idx[ID]-index-1].from] = minimumGraphStack[currentID].to;
                                    minimumGraphTraveled[idx[ID]-index-1] = true;

                                }else
                                {   
                                    node_mapping[minimumGraphContext[idx[ID]].to]    = minimumGraphStack[currentID].to;
                                    node_mapping[minimumGraphContext[idx[ID]].from]  = minimumGraphStack[currentID].from;
                                    minimumGraphTraveled[idx[ID]] = true;
                                }

                                   parentID    =  minimumGraphStackList[parentID].parentID;
                                   currentID   =  minimumGraphStack[currentID].ancestor;
                            }


                             ID  = minimumGraphStackList[i].ID;
                             uint16_t activeNode; 

                             if( minimumGraphStack[ancestorID].to > minimumGraphStack[ancestorID].from )
                             {

                                     if(idx[ID]>index)
                                     {   
                                        activeNode = minimumGraphContext[idx[ID]-index-1].from;
                                     }else
                                     {   
                                        activeNode = minimumGraphContext[idx[ID]].to;
                                     }

                             }else
                             { 
                                    if(idx[ID]>index)
                                    {   
                                        activeNode = minimumGraphContext[idx[ID]-index-1].to;
                                    }else
                                    {   
                                        activeNode = minimumGraphContext[idx[ID]].from;
                                    }
                             }

                               uint16_t min_I    = MAX_UINT_CONSTANT16; 
                               uint16_t min_to   = MAX_UINT_CONSTANT16;
                               uint16_t min_from = 0;
                               uint16_t min_Rank = MAX_UINT_CONSTANT16;

                                 for(int i4 =0;i4<(((index+1)*2));i4++)
                                {



                                        min_I = MAX_UINT_CONSTANT16;

                                        if(idx[i4]>index)
                                        {   
                                             if( minimumGraphTraveled[idx[i4]-index-1]==false )
                                             {
                                    
                                                 if( (minimumGraphContext[ idx[i4]-index-1 ].to == activeNode)&&(node_mapping[minimumGraphContext[ idx[i4]-index-1 ].from] !=MAX_UINT_CONSTANT16) )
                                                 {

                                                      if( node_mapping[minimumGraphContext[ idx[i4]-index-1 ].from] < min_to)
                                                      {
                                                             min_to = node_mapping[minimumGraphContext[ idx[i4]-index-1 ].from];
                                                             min_I =  i4;
                                                             min_Rank = EqualTransitive[min_I];

                                                      }else if( node_mapping[minimumGraphContext[ idx[i4]-index-1 ].from] == min_to )
                                                      {
                                                         if( EqualTransitive[i4] <= min_Rank )
                                                         {
                                                            min_I = i4;
                                                            min_Rank = EqualTransitive[min_I];
                                                         }
                                                      }  
                                                }

                                             } 
                                         }   
                                         else{ 

                                            if (minimumGraphTraveled[idx[i4]]==false )
                                            {

                                                if( (minimumGraphContext[ idx[i4]].from == activeNode)&&(node_mapping[minimumGraphContext[idx[i4]].to] !=MAX_UINT_CONSTANT16) )
                                                {

                                                        if( node_mapping[minimumGraphContext[ idx[i4] ].to] < min_to)
                                                       {
                                                          min_to =  node_mapping[minimumGraphContext[idx[i4]].to];
                                                          min_I  = i4;
                                                          min_Rank = EqualTransitive[min_I];

                                                       }else if(node_mapping[minimumGraphContext[ idx[i4] ].to] == min_to )
                                                       {
                                                         if( EqualTransitive[i4] <= min_Rank )
                                                         {
                                                            min_I = i4;
                                                            min_Rank = EqualTransitive[min_I];
                                                         }
                                                      }

                                                }

                                            }
                                         }


                                         if( min_I!=MAX_UINT_CONSTANT16)
                                         {
                                             if ((minimumGraphStack[stackSize].to > minimumGraphStack[stackSize].from)||(min_to < minimumGraphStack[stackSize].to)||(( min_to == minimumGraphStack[stackSize].to)&&(min_Rank < minimumGraphStack[stackSize].rank ) ))
                                             {
                                                  minimumGraphStack[stackSize].to   = min_to;
                                                  minimumGraphStack[stackSize].from = node_mapping[activeNode];
                                                  minimumGraphStack[stackSize].rank = min_Rank;
                                                  minimumGraphStack[stackSize].end  = minimumGraphStack[stackSize].begin;
                                                  minimumGraphStackList[ minimumGraphStack[stackSize].end ].ID = min_I; 
                                                  minimumGraphStackList[ minimumGraphStack[stackSize].end ].parentID = i;
                                                  minimumGraphStack[stackSize].end  = minimumGraphStack[stackSize].end+1;
                                                  if(minimumGraphStack[stackSize].end >= MAX_Stack_Size)
                                                  goto error; 

                                             }else if( ( min_to == minimumGraphStack[stackSize].to)&&(min_Rank == minimumGraphStack[stackSize].rank ) )
                                             {
                                                  minimumGraphStackList[ minimumGraphStack[stackSize].end ].ID = min_I; 
                                                  minimumGraphStackList[ minimumGraphStack[stackSize].end ].parentID = i;
                                                  minimumGraphStack[stackSize].end = minimumGraphStack[stackSize].end+1;
                                                  if(minimumGraphStack[stackSize].end >= MAX_Stack_Size)
                                                  goto error;

                                             }

                                         }


                                }

                                        //backwardEdge 
                                        if(minimumGraphStack[stackSize].to < minimumGraphStack[stackSize].from)
                                        {
                                            continue;
                                        }

                          





                                             { 
                                                        uint16_t to_;
                                                        uint16_t from_;
                                                        bool activeNodeChanged;
                                                        parentID = i;
                                                        ID  = minimumGraphStackList[parentID].ID;
                                                        if(idx[ID]>index)
                                                        {
                                                             from_ = node_mapping[ minimumGraphContext[idx[ID] - index-1].to   ];    
                                                             to_   = node_mapping[ minimumGraphContext[idx[ID] - index-1].from ];
                                                             if( from_ < to_ )
                                                             {
                                                                 activeNode = minimumGraphContext[idx[ID] - index-1].from;

                                                             }else
                                                             {
                                                                 activeNode = minimumGraphContext[idx[ID] - index-1].to;
                                                             }

                                                       }else
                                                       {
                                                             from_ = node_mapping[ minimumGraphContext[idx[ID]].from  ];
                                                             to_   = node_mapping[ minimumGraphContext[idx[ID]].to    ];

                                                             if( from_ < to_ )
                                                             {
                                                                 activeNode = minimumGraphContext[idx[ID]].to;

                                                             }else
                                                             {
                                                                 activeNode = minimumGraphContext[idx[ID]].from;
                                                             }

                                                      }

                                                     activeNodeChanged = true;

                                                                                 while(true)
                                                                                 { 



                                                                                    min_I =    0xffff;
                                                                                    min_Rank = 0xffff;
                                                                                    min_from = 0;



                                                                                    if(activeNodeChanged ==true )
                                                                                    {

                                                                                            for(int i5 =0;i5<(((index+1)*2));i5++)
                                                                                            {
                                                                                                if(idx[i5]>index)
                                                                                                {

                                                                                                    if( minimumGraphTraveled[idx[i5]-index-1]==false)
                                                                                                    {

                                                                                                        if( (minimumGraphContext[ idx[i5]-index-1 ].to == activeNode)&&(node_mapping[minimumGraphContext[ idx[i5]-index-1 ].from] ==MAX_UINT_CONSTANT16) )
                                                                                                        {
                                                                                                               if( EqualTransitive[i5] <= min_Rank )
                                                                                                                {
                                                                                                                   min_I = i5;
                                                                                                                   min_Rank = EqualTransitive[min_I];
                                                                                                                   min_from = node_mapping[activeNode];
                                                                                                                   if((min_from > minimumGraphStack[stackSize].from)||((min_from == minimumGraphStack[stackSize].from)&&(min_Rank <= minimumGraphStack[stackSize].rank)))
                                                                                                                   {

                                                                                                                       if((minimumGraphStack[stackSize].from == min_from)&&(min_Rank == minimumGraphStack[stackSize].rank))
                                                                                                                       {

                                                                                                                            minimumGraphStackList[ minimumGraphStack[stackSize].end ].ID = min_I; 
                                                                                                                            minimumGraphStackList[ minimumGraphStack[stackSize].end ].parentID = i;
                                                                                                                            minimumGraphStack[stackSize].end = minimumGraphStack[stackSize].end+1;
                                                                                                                            if(minimumGraphStack[stackSize].end >= MAX_Stack_Size)
                                                                                                                             goto error;
                            
                                                                                                                       }else
                                                                                                                       {  
                                                                                                                            minimumGraphStack[stackSize].from = min_from;
                                                                                                                            minimumGraphStack[stackSize].rank = min_Rank;
                                                                                                                            minimumGraphStack[stackSize].end  = minimumGraphStack[stackSize].begin;
                                                                                                                            minimumGraphStackList[ minimumGraphStack[stackSize].end ].ID = min_I; 
                                                                                                                            minimumGraphStackList[ minimumGraphStack[stackSize].end ].parentID = i;
                                                                                                                            minimumGraphStack[stackSize].end  = minimumGraphStack[stackSize].end+1;
                                                                                                                            if(minimumGraphStack[stackSize].end >= MAX_Stack_Size)
                                                                                                                             goto error;

                                                                                                                       }

                                                                                                                   }

                                                                                                                }
                                                                                                        }

                                                                                                    }

                                                                                                }else
                                                                                                {
                                                                                                    if(minimumGraphTraveled[idx[i5]]==false )
                                                                                                    {
                                                                                                        if( (minimumGraphContext[ idx[i5]].from == activeNode)&&(node_mapping[minimumGraphContext[idx[i5]].to] == MAX_UINT_CONSTANT16) )
                                                                                                        {
                                                                                                                if( EqualTransitive[i5] <= min_Rank )
                                                                                                                {
                                                                                                                   min_I = i5;
                                                                                                                   min_Rank = EqualTransitive[min_I];
                                                                                                                   min_from = node_mapping[activeNode];
                                                                                                                  if((min_from > minimumGraphStack[stackSize].from)||((min_from == minimumGraphStack[stackSize].from)&&(min_Rank <= minimumGraphStack[stackSize].rank)))
                                                                                                                   {

                                                                                                                       if((minimumGraphStack[stackSize].from == min_from)&&(min_Rank == minimumGraphStack[stackSize].rank))
                                                                                                                       {

                                                                                                                            minimumGraphStackList[ minimumGraphStack[stackSize].end ].ID = min_I; 
                                                                                                                            minimumGraphStackList[ minimumGraphStack[stackSize].end ].parentID = i;
                                                                                                                            minimumGraphStack[stackSize].end = minimumGraphStack[stackSize].end+1;
                                                                                                                            if(minimumGraphStack[stackSize].end >= MAX_Stack_Size)
                                                                                                                             goto error;
                                                                                                                       }else
                                                                                                                       {  
                                                                                                                            minimumGraphStack[stackSize].from = min_from;
                                                                                                                            minimumGraphStack[stackSize].rank = min_Rank;
                                                                                                                            minimumGraphStack[stackSize].end  = minimumGraphStack[stackSize].begin;
                                                                                                                            minimumGraphStackList[ minimumGraphStack[stackSize].end ].ID = min_I; 
                                                                                                                            minimumGraphStackList[ minimumGraphStack[stackSize].end ].parentID = i;
                                                                                                                            minimumGraphStack[stackSize].end  = minimumGraphStack[stackSize].end+1;
                                                                                                                            if(minimumGraphStack[stackSize].end >= MAX_Stack_Size )
                                                                                                                             goto error;

                                                                                                                       }

                                                                                                                   }

                                                                                                                }
                                                                                                        }

                                                                                                    }
                                                                                                }


                                                                                            }


                                                                                    }

                                                                                    if(( min_I!= MAX_UINT_CONSTANT16 )||(parentID==MAX_UINT_CONSTANT))
                                                                                    {
                                                                                        break;
                                                                                    }

                                                                           


                                                                                    activeNodeChanged =false;
                                                                                    ID = minimumGraphStackList[parentID].ID;

                                                                                    if(idx[ID]>index)
                                                                                    {
                                                                                         from_ = node_mapping[ minimumGraphContext[idx[ID] - index-1].to   ];    
                                                                                         to_   = node_mapping[ minimumGraphContext[idx[ID] - index-1].from ];
                                                                                         if( from_ < to_ )
                                                                                         {
                                                                                             if (activeNode == minimumGraphContext[idx[ID] - index-1].from)
                                                                                             {
                                                                                                  activeNode = minimumGraphContext[idx[ID] - index-1].to;
                                                                                                  activeNodeChanged =true;
                                                                                             }

                                                                                         }

                                                                                    }else
                                                                                    {
                                                                                         from_ = node_mapping[ minimumGraphContext[idx[ID]].from   ];
                                                                                         to_   = node_mapping[ minimumGraphContext[idx[ID]].to     ];

                                                                                         if( from_ < to_ )
                                                                                         {

                                                                                             if (activeNode == minimumGraphContext[idx[ID]].to)
                                                                                             {
                                                                                                  activeNode = minimumGraphContext[idx[ID]].from;
                                                                                                  activeNodeChanged =true;
                                                                                             }
                                                                                            
                                                                                         }

                                                                                    }

                                                                                        parentID    =  minimumGraphStackList[parentID].parentID;
                                                                                }



                                                                        if( min_I!=MAX_UINT_CONSTANT16)
                                                                        {
                                                                             minimumGraphStack[stackSize].to=std::max(minimumGraphStack[ancestorID].to,minimumGraphStack[ancestorID].from)+1;
                                                                        }

                                                    }  
     
                        }



                        if(isTraced==false)
                        {
                             for(unsigned int j = minimumGraphStack[stackSize].begin;j<minimumGraphStack[stackSize].end;j++)
                            {
                              

                                if(((idx[minimumGraphStackList[j].ID]) == MatchedID )&&((idx[minimumGraphStackList[minimumGraphStackList[j].parentID].ID]) == (MatchedID-1) ))
                               {
                                  if (( minimumGraphContext[MatchedID].to == minimumGraphStack[stackSize].to )&&( minimumGraphContext[MatchedID].from == minimumGraphStack[stackSize].from ))
                                  {

                                     isMatched = true;
                                     break;
                                 }
                               }

                           }

                          if(isMatched == false )
                          {
                             return false;
                          }

                          isMatched = false;
                          MatchedID = MatchedID+1;

                     }

                  }    

                      if(isTraced==false)
                     {
                        minimumGraphStacksize=stackSize;
                        return true;
                     }else
                     {
                        minimumCodeTrace.resize(stackSize+1);

                        for(int i=0;i<(stackSize+1);i++)
                        {
                            minimumCodeTrace[i].to =    minimumGraphStack[i].to;
                            minimumCodeTrace[i].from  = minimumGraphStack[i].from;

                            unsigned int ID =  idx[minimumGraphStackList[ minimumGraphStack[i].begin].ID];

                            if(ID > index)
                            {
                                minimumCodeTrace[i].elabel      = minimumGraphContext[ID - index - 1].elabel;
                                minimumCodeTrace[i].vlabelTo    = minimumGraphContext[ID - index - 1].vlabelFrom;  
                                minimumCodeTrace[i].vlabelFrom  = minimumGraphContext[ID - index - 1].vlabelTo;

                            }else
                            {
                                minimumCodeTrace[i].elabel      = minimumGraphContext[ID].elabel;
                                minimumCodeTrace[i].vlabelTo    = minimumGraphContext[ID].vlabelTo;
                                minimumCodeTrace[i].vlabelFrom  = minimumGraphContext[ID].vlabelFrom;
                            }

                        }

                        minimumGraphStacksize=stackSize;
                        return true;
                     }
 

              }

         error:
            {       if(isTraced==false)
                    printf("warning: In minimum DFS code checking, the frequent subgraph with highly symmetrical has been discarded.\n");
                    else
                    minimumCodeTrace.resize(0);
                    return false;
            } 

}









