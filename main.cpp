#include "ComStruct/ComStruct.h"
#include <fstream>
#include <iostream>








void LoadM(const char *filename, std::vector< std::vector< unsigned int > >  &Map, double Threshold ) {         
    std::vector< unsigned int > location_u;
    std::vector< unsigned int > location_m;
    std::vector<double> values;                    

    std::ifstream file(filename);                  
    unsigned int a, b;
    double c; 
    unsigned max = 0;                              
    while(file >> a >> b >> c) {   
         if(c > Threshold) 
         {
            location_u.push_back(a);              
            location_m.push_back(b);
         }     

         if(max < b )  
         {
            max = b;
         }                                         
    }    

      Map.resize(max+1);

      for(int i=0;i< location_u.size();i++)
      {
          Map[location_m[i]].push_back(location_u[i]); 
      }                                   
                                    
}   

 ComStruct  gDebug; 
 //FILE *  Output_Debug;


int main( int argc, char **argv)
{
 
     
     unsigned int Count=0;
     std::vector< std::vector< unsigned int > >  Map;
     ComStruct  g;
     UniqueGraphSet H; 
     
     


     Map.resize(0);

   //  Output_Debug = fopen("debug","w+");

     if(argc<3)
     {
        printf("./GraphGen  GraphSet Frequency FrequentGraphDataSet [-d  FrequentGraphDataSetIndex]  [ -group SelectedIndex] [-T] \n");
        printf("./GraphGen  INTERSECT GraphDataSet1   GraphDataSet2  ..  -o output \n");
        printf("./GraphGen  EXCLUDE GraphDataSet1  GraphDataSet1 -o output  \n");
        printf("./GraphGen  COUNT GraphDataSet \n");
        printf("./GraphGen  SUMMARY GraphDataSet \n");
        printf("./GraphGen  MATCH GraphDataSet  MatchGraphFile output [-Verbose]\n");
        printf("./GraphGen  SELECT  GraphSet [-N] FrequentGraphDataSetIndex output\n");

        return 0;
     }


     FILE * output2 = NULL;


     for(int i =0;i<argc;i++)
     {



       if( strcmp(argv[i],"INTERSECT")==0)
       {
           int j = 1;
           while(  argv[i+j] !=NULL )
           {
                if(strcmp(argv[i+j],"-o")==0)
                {

                    printf("H.GraphSet.size():%d \n",H.GraphSet.size());
                    FILE * output1 =  fopen(argv[i+j+1],"w+");

                    for(int i_=0;i_<H.GraphSet.size();i_++)
                    {
                       g.OutPutGraph(H.GraphSet[i_],output1);
                    }

                    break;

                }else
                {

                    FILE * input1 =  fopen(argv[i+j],"r");

                    if(input1!=NULL)
                    {
                        g.GraphRead(input1);

                        for(int i_=0;i_<g.Sample.size();i_++)
                        {
                          // if( H.MPinsert(g.Sample[i_])==false)
			  // {
			  //     printf("%d\n",i_);
			  // }
			  H.MPinsert(g.Sample[i_]);
                        }

                          g.Sample.resize(0);
                          fclose(input1);
                    }else
                     {
                         printf("Fail to open the GraphFiles\n");
                         return 0;
                     }
                }

                j=j+1;
           }
           return 0; 
        }



        if( strcmp(argv[i],"EXCLUDE")==0)
       {
          


           FILE * input1 =  fopen(argv[i+1],"r");

           if(input1!=NULL)
           {
                     g.GraphRead(input1);

                     for(int i_=0;i_<g.Sample.size();i_++)
                     {
                          H.MPinsert(g.Sample[i_]);
                     }

                          g.Sample.resize(0);
                          fclose(input1);
           }else
           {
                    printf("Fail to open the GraphFiles\n");
                    return 0;
           }



           FILE * input2 =  fopen(argv[i+2],"r");



           if(input2!=NULL)
           {

               if(strcmp(argv[i+3],"-o")==0)
               {

                FILE * output3 =  fopen(argv[i+4],"w+");

                if(output3!=NULL)
                {
                    g.GraphRead(input2);

                   for(int i_=0;i_<g.Sample.size();i_++)
                   {
                      if( H.MPfind(g.Sample[i_])==MAX_UINT_CONSTANT)
                      {
                          g.OutPutGraph(H.GraphSet[i_],output3);
                      }
                   }

                }

                 g.Sample.resize(0);
                 fclose(input2);

              }
           }else
           {
                    printf("Fail to open the GraphFiles\n");
                    return 0;
           }


           return 0; 
        }








         if( strcmp(argv[i],"COUNT")==0)
        {

            FILE * input1 =  fopen(argv[i+1],"r");

            if(input1!=NULL)
            {
                g.GraphRead(input1);

                printf("The number of graphs is %d\n",g.Sample.size()); 
                fclose(input1);
            }
            return 0;

        }


        if( strcmp(argv[i],"SUMMARY")==0)
        {

            FILE * input1 =  fopen(argv[i+1],"r");

            if(input1!=NULL)
            {
                g.GraphRead(input1);

                for(int i=0;i<g.Sample.size();i++)
                {
                    printf("%d  %d\n",g.Sample[i].vertex_List.size(),g.Sample[i].EdgeSize); 

                }

                
                fclose(input1);
            }
            return 0;

        }


        if( strcmp(argv[i],"SELECT")==0)
        {

            unsigned int Order1;
            unsigned int Order2;
            char buffer[1024];
            
            FILE * input1  =  fopen(argv[i+1],"r");


            if(input1!=NULL)
            {
                g.GraphRead(input1);
                fclose(input1);
            }else
            {
                return 0;
            }


            if(strcmp(argv[i+2],"-N")==0)
	        {
                FILE * output3 =  fopen(argv[i+4],"w+");
                FILE * fp =  fopen(argv[i+3],"r");

                while(!feof(fp))
                {
                    fscanf(fp,"%[\b|\t|,]*",buffer);
                    if(fscanf(fp,"%u",&Order1)==EOF) break;
                    fscanf(fp,"%[\b|\t|,]*",buffer);
                    if(fscanf(fp,"%u",&Order2)==EOF) break;
                    fscanf(fp,"%[\b|\t|,|\n]*",buffer); 


                    if(Order1<g.Sample.size())
                    {
                       g.OutPutGraphNode(g.Sample[Order1],output3,Order2);
                    }
                }  
	       
	    
	        }else
	        {

                FILE * output3 =  fopen(argv[i+3],"w+");
                FILE * fp =  fopen(argv[i+2],"r");

                while(!feof(fp))
                {
                       fscanf(fp,"%[\b|\t|,]*",buffer);
                    if(fscanf(fp,"%u",&Order1)==EOF) break;
                      fscanf(fp,"%[\b|\t|,|\n]*",buffer); 
                    if(Order1<g.Sample.size())
                    {
                       g.OutPutGraph(g.Sample[Order1],output3);
                    }
                 }    
	        }

            return 0;

        }




    
       if( strcmp(argv[i],"-T")==0)
       {
          g.Trim = true;
       }





        if( strcmp(argv[i],"MATCH")==0)
        {

            ComStruct  Sub_g; 
            std::string str_flag ("-Verbose");


            g.isSelected = false;
            g.isTrace=false;

            FILE * input1 =  fopen(argv[i+1],"r");
            FILE * input2 =  fopen(argv[i+2],"r");
            FILE * output3 =  fopen(argv[i+3],"w+");



            Sub_g.GraphReadRemoveDuplicatedEdge(input2);


            if(input1!=NULL)
            {
                g.GraphReadRemoveDuplicatedEdge(input1);
            } 


	    bool  Verbose = false;

	    if(argv[i+4]!=NULL)
	       if(str_flag.compare(argv[i+4])==0)
	       {
	            Verbose=true;
	       }


        if(Verbose)
        {

             for(int i=0;i<Sub_g.Sample.size();i++)
             {
                    g.isTrace = true;

                    if(g.graphBulkMatch(Sub_g.Sample[i]))
                   { 
			 for(int j=0;j<g.MatchedGraphsIDArray.size();j++)
                      {

                        for(int j1=0;j1<g.TraceArray[j].size();j1++)
                        {
                            std::vector<unsigned int> Index;
                            Index.resize(g.TraceArray[j][j1].size());

                            for(int i=0;i<g.TraceSingle[0].size();i++)
                            {
                                Index[g.TraceSingle[0][i]] = i;
                            }
                           assert(g.TraceSingle[0].size()==g.TraceArray[j][j1].size());
                           for(int j2=0;j2<g.TraceArray[j][j1].size();j2++)
                           {
                               fprintf(output3,"%d   ",i);
                               fprintf(output3,"%d   ",g.MatchedGraphsIDArray[j]);
                               fprintf(output3,"%d   ",j1);
                               fprintf(output3,"%d\n",g.TraceArray[j][j1][Index[j2]]);
                           }
                        }
                      }
                  }    
               }
          }
        else{
             for(int i=0;i<Sub_g.Sample.size();i++)
             {
                    if(g.graphBulkMatch(Sub_g.Sample[i]))
                   {    
                    
                       for(int j=0;j<g.MatchedGraphsIDArray.size();j++)
                      {
                         fprintf(output3,"%d   ",i);
                         fprintf(output3,"%d   ",g.MatchedGraphsIDArray[j]);
                         fprintf(output3,"%lf\n",(g.MatchedGraphsIDCountArray[j]*1.0)/g.MatchedSingleGraphCount);
                      }
             
                  }
                  
             }
         }

           fclose(input1);
           fclose(input2);
           fclose(output3);

            return 0;

        }






       


     }






     FILE * input1 =  fopen(argv[1],"r");
         
     //g.isTrace = false;

     double Frequency = atof(argv[2]);

     FILE * output1 =  fopen(argv[3],"w+");



       


     FILE * subsetFile = NULL;

 


 







     for(int i =0;i<argc-1;i++)
     {
       if( strcmp(argv[i],"-group")==0)
       {
             subsetFile = fopen(argv[i+1],"r");

             if(subsetFile != NULL)
            {
               fclose(subsetFile);
               LoadM(argv[i+1],Map,0.95);
            }

            break;
        }
     }


    // g.isSelected = false; 

     /*for(int i =0;i<argc-1;i++)
     {
       if( strcmp(argv[i],"-l1")==0)
       {
            g.IsExtract = true;
            break;
        }
     }
     */

    
     

     g.GraphReadRemoveDuplicatedEdge(input1);


if(Map.size()!=0)
{
  for(int i=0;i< Map.size();i++)
  {
    
     g.SubSet = Map[i];
     g.isSelected = true; 
     g.minsup=(int)(((float)g.SubSet.size())*Frequency);
     g.Verbose = true;
     if(g.minsup<8)
     continue;
     g.FrequentGraphMining();
     //g.RemoveDuplicates();
   
     for(int i=0;i<g.FrequentGraphs.size();i++)
     {
         H.MPinsert(g.FrequentGraphs[i]);
     }

  }

    ComStruct  g2; 
    g.isTrace=false;
    g.isSelected = false; 

    printf("H.GraphSet.size():%d \n",H.GraphSet.size());

    for(int i=0;i<H.GraphSet.size();i++)
    {
       g2.OutPutGraph(H.GraphSet[i],output1);
    }


    if(output2!=NULL)
    {
        for(int i=0;i<H.GraphSet.size();i++)
       {

            if(g.graphBulkMatch(H.GraphSet[i]))
           {
               for(int j=0;j<g.MatchedGraphsIDArray.size();j++)
               {
                    fprintf(output2,"%d   ",i);
                    fprintf(output2,"%d   ",g.MatchedGraphsIDArray[j]);
                    fprintf(output2,"%lf\n",(g.MatchedGraphsIDCountArray[j]*1.0)/g.MatchedSingleGraphCount);
               }

           }
        }
    }

}else
{

    g.isSelected = false;
    g.minsup=(int)(Frequency*g.Sample.size());
    g.FrequentGraphMining();
    // g.RemoveDuplicates();
    unsigned int index =0;

 

    for(int i=0;i<g.FrequentGraphs.size();i++)
    {
        //  if(g.FrequentGraphsIDArray[i]!=0)
          {

               g.isTrace=false;      
               g.OutPutGraph(g.FrequentGraphs[i],output1);
               index = index+1;

         }
    }
    printf("The size of frequent graphs:%d\n",index);

    index =0;
    
    if(output2!=NULL)
    {

        for(int i=0;i<g.FrequentGraphs.size();i++)
        {

            index = index+1;

            if(g.graphBulkMatch(g.FrequentGraphs[i]))
           {    
                    
               for(int j=0;j<g.MatchedGraphsIDArray.size();j++)
               {
                  fprintf(output2,"%d   ",index);
                  fprintf(output2,"%d   ",g.MatchedGraphsIDArray[j]);
                  fprintf(output2,"%lf\n",(g.MatchedGraphsIDCountArray[j]*1.0)/g.MatchedSingleGraphCount);
               }
          }

        }
    }




}




return 0;

/*
     printf("Run successful:%d\n",g.FrequentGraphs.size());
     for(int i=0;i<g.FrequentGraphsIDArray.size();i++)
     {
          if(g.FrequentGraphsIDArray[i].size()!=0)
          {
               Count= Count+1;
          }
     }

     printf("NumberAfterFiltered:%d\n",Count);

     UniqueGraphSet Verifcation; 


     unsigned int index = 0;


     for(int i=0;i<g.FrequentGraphsIDArray.size();i++)
     {
          if(g.FrequentGraphsIDArray[i].size()!=0)
          {

               g.isTrace=true;
              
             

               if(g.graphBulkMatch(g.FrequentGraphs[i]))
               {    
                    
                    for(int j=0;j<g.MatchedGraphsIDArray.size();j++)
                    {
                       fprintf(output1,"%d   ",index);
                       fprintf(output1,"%d   ",g.MatchedGraphsIDArray[j]);
                       fprintf(output1,"%lf\n",(g.MatchedGraphsIDCountArray[j]*1.0)/g.MatchedSingleGraphCount);
                    }

                    assert(g.MatchedGraphsIDArray.size()==g.FrequentGraphsIDArray[i].size());
             
               }
               index = index +1;
               */
               
          //   g.OutPutGraph(g.FrequentGraphs[i],output1); 

              // if(Verifcation.MPinsert(g.FrequentGraphs[i])==false)
               //{
                 //   printf("Failure%d\n",i);
               //}


             //  g.OutPutGraph(g.FrequentGraphs[i],output1); 


            /*   
             unsigned int unmatch=0;
             std::vector<unsigned int> debugTxT;

             debugTxT.resize(0);

             g.OutPutGraph(g.FrequentGraphs[i],output1); 


             for(int j=0;j<g.Sample.size();j++)
             { 

                  if(g.IsConteined(g.Sample[j],g.FrequentGraphs[i]))
                  {
                      unmatch = unmatch+1;
                      debugTxT.push_back(j);
                  }


               }
		  if(unmatch!=g.FrequentGraphsIDArray[i].size())
                  {
                 

                       std::ofstream f;
                       f.open("vector1.txt");
                       for(int i=0;i<debugTxT.size();i++)
                       {
                            f<< debugTxT[i]<<"\n";
                       }

                       f.close();
                       
                       std::ofstream f2;
                       f2.open("vector2.txt");
                       for(int j=0;j<g.FrequentGraphsIDArray[i].size();j++)
                       {
                            f2<<g.FrequentGraphsIDArray[i][j]<<"\n";
                       }

                       f2.close();

                     printf("Failure%d\n",(g.FrequentGraphsIDArray[i].size() - unmatch));
                  }
                    

          }
     }

*/



     printf("TestSuccessful\n");

     fclose(output1);



}

