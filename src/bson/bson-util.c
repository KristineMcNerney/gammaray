/*****************************************************************************
 * bson-util.c                                                               *
 *                                                                           *
 * This file implements utility functions for the BSON library that don't fit*
 * the normal BSON API.                                                      *
 *                                                                           *
 *                                                                           *
 *   Authors: Wolfgang Richter <wolf@cs.cmu.edu>                             *
 *                                                                           *
 *                                                                           *
 *   Copyright 2013 Carnegie Mellon University                               *
 *                                                                           *
 *   Licensed under the Apache License, Version 2.0 (the "License");         *
 *   you may not use this file except in compliance with the License.        *
 *   You may obtain a copy of the License at                                 *
 *                                                                           *
 *       http://www.apache.org/licenses/LICENSE-2.0                          *
 *                                                                           *
 *   Unless required by applicable law or agreed to in writing, software     *
 *   distributed under the License is distributed on an "AS IS" BASIS,       *
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*
 *   See the License for the specific language governing permissions and     *
 *   limitations under the License.                                          *
 *****************************************************************************/
#include "__bson.h"
#include "bson.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int bson_print(FILE* stream, struct bson_info* bson)
{
    struct bson_info* embedded;
    struct bson_kv v1;
    struct bson_kv v2;
    time_t utctime;

    fprintf(stream, "{\n");

    while (bson_deserialize(bson, &v1, &v2) == 1) 
    {
        fprintf(stream, "\t'%s' : ", v1.key);
        switch (v1.type)
        {
            case BSON_DOUBLE:
                fprintf(stream, "[BSON_DOUBLE] %f\n", *((double *) v1.data));
                break;
            case BSON_STRING:
                fprintf(stream, "[BSON_STRING, %"PRIu32"] '", v1.size);
                fwrite((uint8_t *) v1.data, v1.size, 1, stream);
                fprintf(stream, "'\n");
                break;
            case BSON_EMBEDDED_DOCUMENT:
                fprintf(stream, "[BSON_EMBEDDED_DOCUMENT] \n");
                break;
            case BSON_ARRAY:
                fprintf(stream, "[BSON_ARRAY] ");
                embedded = bson_init();
                embedded->buffer = malloc(v2.size);
                memcpy(embedded->buffer, v2.data, v2.size);
                bson_make_readable(embedded);
                bson_print(stream, embedded);
                bson_cleanup(embedded);
                break;
            case BSON_BINARY:
                fprintf(stream, "[BSON_BINARY, %"PRIu32"] ", v1.size);
                hexdump((uint8_t *) v1.data, v1.size);
                break;
            case BSON_UNDEFINED:
                fprintf(stream, "[BSON_UNDEFINED] \n");
                break;
            case BSON_OBJECTID:
                fprintf(stream, "[BSON_OBJECTID] \n");
                break;
            case BSON_BOOLEAN:
                fprintf(stream, "[BSON_BOOLEAN] ");
                if (*((uint8_t *) v1.data))
                    fprintf(stream, "true\n");
                else
                    fprintf(stream, "false\n");
                break;
            case BSON_UTC_DATETIME:
                fprintf(stream, "[BSON_UTC_DATETIME] ");
                utctime = *((int64_t *) v1.data) / 1000; 
                fprintf(stream, "%s\n", asctime(gmtime(&utctime)));
                break;
            case BSON_NULL:
                fprintf(stream, "[BSON_NULL] \n");
                break;
            case BSON_REGEX:
                fprintf(stream, "[BSON_REGEX] \n");
                break;
            case BSON_DBPOINTER:
                fprintf(stream, "[BSON_DBPOINTER] \n");
                break;
            case BSON_JS:
                fprintf(stream, "[BSON_JS] \n");
                break;
            case BSON_SYMBOL:
                fprintf(stream, "[BSON_SYMBOL] \n");
                break;
            case BSON_JS_CODE:
                fprintf(stream, "[BSON_JS_CODE] \n");
                break;
            case BSON_INT32:
                fprintf(stream, "[BSON_INT32] %"PRId32"\n",
                                *((int32_t *) v1.data));
                break;
            case BSON_TIMESTAMP:
                fprintf(stream, "[BSON_TIMESTAMP] \n");
                break;
            case BSON_INT64:
                fprintf(stream, "[BSON_INT64] %"PRId64"\n",
                                *((int64_t *) v1.data));
                break;
            case BSON_MIN:
                fprintf(stream, "[BSON_MIN] \n");
                break;
            case BSON_MAX:
                fprintf(stream, "[BSON_MAX] \n");
                break;
            default:
                fprintf(stream, "\t'%s' : print not implemented\n", v1.key);
                break;
        };
    }

    fprintf (stream, "}\n");

    return EXIT_SUCCESS;
}
