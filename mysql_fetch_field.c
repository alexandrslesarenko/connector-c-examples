/* Copyright (C) 2005 - 2019 Hartmut Holzgraefe <hartmut@php.net>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
*/

/*
 * MySQL C client API example: mysql_fetch_fields()
 *
 * see also http://mysql.com/mysql_fetch_fields
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>

#include <mysql.h>

int main(int argc, char **argv) 
{
  MYSQL *mysql = NULL;

  if (mysql_library_init(argc, argv, NULL)) {
    fprintf(stderr, "could not initialize MySQL client library\n");
    exit(1);
  }
 
  mysql = mysql_init(mysql);

  if (!mysql) {
    puts("Init faild, out of memory?");
    return EXIT_FAILURE;
  }
        
  mysql_options(mysql, MYSQL_READ_DEFAULT_FILE, (void *)"./my.cnf");

  if (!mysql_real_connect(mysql,       /* MYSQL structure to use */
			  NULL,  /* server hostname or IP address */ 
			  NULL,  /* mysql user */
			  NULL,   /* password */
			  NULL,    /* default database to use, NULL for none */
			  0,           /* port number, 0 for default */
			  NULL,        /* socket file or named pipe name */
			  CLIENT_FOUND_ROWS /* connection flags */ )) {
      printf("Failed to connect to database: Error: %s\n",
              mysql_error(mysql));
  } else {                
    mysql_query(mysql, "SET NAMES utf8");
    if (mysql_query(mysql, "SELECT * FROM city LIMIT 1")) {
      printf("Query failed: %s\n", mysql_error(mysql));
    } else {
      MYSQL_RES *result = mysql_store_result(mysql);

      if (!result) {
	printf("Couldn't get results set: %s\n", mysql_error(mysql));
      } else {
	MYSQL_FIELD *field;
	int i = 0;

	while (NULL != (field = mysql_fetch_field(result))) {
	  printf("FIELD #%d\n", ++i);
	  printf("  %-20s %s\n", "Field name", field->name);
#if MYSQL_VERSION_ID >= 40100
	  printf("  %-20s %s\n", "Original name", field->org_name);
#endif
	  printf("  %-20s %s\n", "From table", field->table);
	  printf("  %-20s %s\n", "Original name", field->org_table);
	  printf("  %-20s %s\n", "Database", field->db);
#if MYSQL_VERSION_ID >= 40100
	  printf("  %-20s %s\n", "Catalog", field->catalog);
#endif
	  printf("  %-20s %s\n", "Default", field->def);
	  printf("  %-20s %lu\n", "CREATE field length", field->length);
	  printf("  %-20s %lu\n", "MAX field lengt", field->max_length);
#if MYSQL_VERSION_ID >= 40100
	  printf("  %-20s %u\n", "Field name length", field->name_length);
	  printf("  %-20s %u\n", "Original name length", field->org_name_length);
	  printf("  %-20s %u\n", "Table name length", field->table_length);
	  printf("  %-20s %u\n", "Original name length", field->org_table_length);
	  printf("  %-20s %u\n", "DB name length", field->db_length);
	  printf("  %-20s %u\n", "Catalog name length", field->catalog_length);
	  printf("  %-20s %u\n", "Default length", field->def_length);
#endif
	  /* TODO: decimals */
	  printf("\n");
	}
      }
    }
  }
        
  mysql_close(mysql);

  mysql_library_end();
  
  return EXIT_SUCCESS;
}
