/* ********************************************************
 * athor: waponx
 * email: 18826419224@163.com
 * time : 8/25/2015
 * ********************************************************/

#ifndef _SERVER_VALUE_H_
#define _SERVER_VALUE_H_
#ifndef DISALLOW_COPY_AND_ASSIGN

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName &);\
	void operator=(const TypeName &)
#endif	//  ! DISALLOW_COPY_AND_ASSIGN

//  the number of subthreads which are created.
const int NTHREADS = 10;

//  socket waiting queue's length.
const int QLEN = 10;

//  the buffer size.
const int BUFFSIZE = 4096;

#endif // !  SERVER_VALUE_H_
