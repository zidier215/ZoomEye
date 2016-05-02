#include <stdio.h>
#include <string.h>
#include "utils.h"
#include <libxml/parser.h>
#include <libxml/tree.h>

#define MY_ENCODING_TYPE "UTF-8"

int creat_xml_file_save_token( char *filename, char *token )
{

  xmlDocPtr doc = NULL;       /* document pointer */
  xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;/* node pointers */

  //1. Creates a new document, a node and set it as a root node
  doc = xmlNewDoc(BAD_CAST "1.0");

  root_node = xmlNewNode(NULL, BAD_CAST "config");
  xmlDocSetRootElement(doc, root_node);

  //2. creates a new node, which is "attached" as child node of root_node node.
  xmlNewChild(root_node, NULL, BAD_CAST "date",BAD_CAST "2016-04-14");
  xmlNewChild(root_node, NULL, BAD_CAST "time",BAD_CAST "19:29:39");

  //3. Here goes another way to create nodes.
  node = xmlNewNode(NULL, BAD_CAST "token");
  node1 = xmlNewText(BAD_CAST"tmpvalue");
  xmlAddChild(node, node1);
  xmlAddChild(root_node, node);

  //4. Dumping document to stdio or file
  xmlSaveFormatFileEnc("savefile", doc, MY_ENCODING_TYPE, 1);

  /*5. free the document */
  xmlFreeDoc(doc);
  xmlCleanupParser();
  xmlMemoryDump();//debug memory for regression tests

  return(0);

}
int update_xml_file_label(char *filename,char *root_elemt, char *change_label, char *change_label_val)
{

    xmlDocPtr doc;              //定义解析文档指针  
    xmlNodePtr curNode, rootNode;         //定义节点指针（在各个节点之间移动）  
    //char* szDocName = argv[1];  //保存xml文档名,该文档名在运行程序到时候输入。  

    doc = xmlReadFile(filename, MY_ENCODING_TYPE, XML_PARSE_RECOVER);   //解析文档  
    if (NULL == doc)  
    {  
        fprintf(stderr, "Document not parsed successfully.\n");  
          
        return -1;  
    }  

    rootNode = xmlDocGetRootElement(doc);  //确定文档根元素  
    if (NULL == rootNode)  
    {  
        fprintf(stderr, "Empty Document.\n");  
        xmlFreeDoc(doc);        //释放文件  

        return -1;  
    }  

    if (xmlStrcmp(rootNode->name, (const xmlChar*)root_elemt))   //确认根元素是否为“log4c”  
    {  
        fprintf(stderr, "Document of wrong type. root node != %s\n", root_elemt);  
        xmlFreeDoc(doc); 
        return -1;  
    }  
    xmlNodePtr save_f = NULL;
    xmlNodePtr node = NULL, node1 = NULL;
    curNode = rootNode->xmlChildrenNode;  
    save_f =  curNode;

    while (NULL != curNode) //遍历所有节点  
    {  
        //获取名称为category的节点  
        if (!xmlStrcmp(curNode->name, (const xmlChar*)(change_label)))  
        {  

            //strcpy((char *)curNode->children->content, change_label_val);
          //xmlNodePtr node_to_del = node;
          //node = node->next;   
          //xmlUnlinkNode( node_to_del );
          //xmlFreeNode( node_to_del );

          node = xmlNewNode(NULL, BAD_CAST change_label);
          node1 = xmlNewText(BAD_CAST change_label_val);
          xmlAddChild(node, node1);
          //xmlAddChild(root_node, node);*/

          xmlReplaceNode(curNode, node);
          //xmlNewChild(rootNode, NULL, BAD_CAST change_label,BAD_CAST change_label_val);
        }  
        
        save_f =  curNode;
        curNode = curNode->next;  
    }  

    //保存文档到原文档中  
    xmlSaveFile(filename, doc);  

  return 0;
}

int get_xml_token_label(char *filename, char *root_name, char *date , char *time, char *token)
{
  xmlDocPtr doc;   //定义解析文档指针
  xmlNodePtr cur; //定义结点指针(你需要它为了在各个结点间移动)
  xmlChar *key;

  doc = xmlReadFile(filename, MY_ENCODING_TYPE, 256); //解析文件

  /*
   * 检查解析文档是否成功，如果不成功，libxml将指一个注册的错误并停止。
   * 一个常见错误是不适当的编码。XML标准文档除了用UTF-8或UTF-16外还可用其它编码保存。
   * 如果文档是这样，libxml将自动地为你转换到UTF-8。更多关于XML编码信息包含在XML标准中。
   */
  if (doc == NULL ) 
  {
    fprintf(stderr,"Document not parsed successfully. /n");
    return 0;
  }

  cur = xmlDocGetRootElement(doc); //确定文档根元素

  /*检查确认当前文档中包含内容*/
  if (cur == NULL) 
  {
    fprintf(stderr,"empty document/n");
    xmlFreeDoc(doc);
    return 0;
  }

  /*在这个例子中，我们需要确认文档是正确的类型。“root”是在这个示例中使用文档的根类型。*/
  if (xmlStrcmp(cur->name, (const xmlChar *)root_name)) 
  {
      fprintf(stderr,"document of the wrong type, root node != root");
      xmlFreeDoc(doc);
      return 0;
  }

  cur = cur->xmlChildrenNode;
  while(cur!=NULL) 
  {
    if ((!xmlStrcmp(cur->name, (const xmlChar *)"date"))) 
    {
      key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      printf("keyword: %s/n", key);
      sprintf(date,"%s", key);
      xmlFree(key);
    }
    else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"time"))) 
    {
      key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      printf("keyword: %s/n", key);
      sprintf(time,"%s", key);
      xmlFree(key);
    }
    else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"token"))) 
    {
      key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      printf("keyword: %s/n", key);
      sprintf(token,"%s", key);
      xmlFree(key);
    }
    cur = cur->next;
  }

  xmlFreeDoc(doc);
  return (0); 

}
int update_xml_all_label(char * filename, char *root_name, char *token)
{
    //get time set with
    //creat_xml_file_save_token("savefile", "mytoken");
    char date[128]="";
    char time[128]="";
    
    printf("token:%s\n", token);
    get_time_ms_str(date , time);
    update_xml_file_label("savefile", root_name, "date", date);
    update_xml_file_label("savefile", root_name, "time", time);
    update_xml_file_label("savefile", root_name, "token", token);

    return 0;
}

#if 0
int main()
{
  creat_xml_file_save_token("savefile", "mytoken");
  update_xml_file_label("savefile", "config", "date", "1970:90:89");
  update_xml_file_label("savefile", "config", "time", "19:29:32");
  update_xml_file_label("savefile", "config", "token", "1111111111111111");
  return 0;
}
#endif
