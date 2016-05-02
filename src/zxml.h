#ifndef __ZXML_H__
#define __ZXML_H__

extern int creat_xml_file_save_token( char *filename, char *token );
extern int update_xml_file_label(char *filename,char *root_elemt, char *change_label, char *change_label_val);
extern int update_xml_all_label(char * filename, char *root_name, char *token);

extern int get_xml_token_label(char *filename, char *root_name, char *date , char *time, char *token);
#endif
