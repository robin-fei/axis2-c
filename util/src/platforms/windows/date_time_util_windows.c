/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <platforms/windows/axis2_date_time_util_windows.h>

AXIS2_EXTERN int AXIS2_CALL axis2_platform_get_milliseconds() 
{
   struct _timeb timebuffer;
   char *timeline;
   int milliseconds = 0; 

   _ftime( &timebuffer );
   timeline = ctime( & ( timebuffer.time ) );
   milliseconds = timebuffer.millitm;
    
   return milliseconds; 

/* printf( "The time is %.19s.%hu %s", timeline, timebuffer.millitm, &timeline[20] );*/
}
