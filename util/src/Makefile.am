SUBDIRS = platforms/unix minizip
lib_LTLIBRARIES = libaxutil.la
libaxutil_la_SOURCES = hash.c \
                        allocator.c \
                        env.c \
                        error.c \
                        stream.c \
                        log.c \
                        string.c \
                        string_util.c \
                        qname.c \
                        array_list.c \
                        linked_list.c \
                        utils.c \
                        dir_handler.c \
                        file_handler.c \
                        class_loader.c\
                        network_handler.c \
                        file.c\
                        uuid_gen.c\
                        thread_pool.c \
                        property.c \
                        types.c \
                        param.c \
                        param_container.c \
                        dll_desc.c\
                        url.c\
                        stack.c \
                        generic_obj.c \
                        base64.c \
                        uri.c \
                        date_time.c \
                        base64_binary.c \
                        properties.c \
						rand.c \
						date_time_util.c \
						version.c
                        
libaxutil_la_LIBADD =	$(top_builddir)/src/platforms/unix/libaxis2_unix.la \
                    		$(top_builddir)/src/minizip/libaxis2_minizip.la

INCLUDES = -I$(top_builddir)/include

EXTRA_DIST=platforms/windows string_util.c
