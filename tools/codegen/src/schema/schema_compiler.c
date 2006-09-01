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

#include <w2c_schema_compiler_options.h>
#include <w2c_schema_consts.h>
#include <w2c_schema_compiler.h>
#include <w2c_schema_writer_meta_info.h>
#include <w2c_schema_writer.h>
#include <w2c_messages.h>

#include <xml_schema_includes.h>
#include <axis2_hash.h>

/**
 * @brief
 */
typedef struct w2c_schema_compiler_impl
{
    w2c_schema_compiler_t compiler;

    w2c_schema_compiler_options_t *compiler_options;
    w2c_schema_writer_t *writer;
    /** maps */
    axis2_hash_t *processed_type_map;
    axis2_hash_t *processed_ele_map;
    axis2_hash_t *loaded_schema_map;
    axis2_hash_t *processed_anonymous_complex_typemap;
    axis2_hash_t *processed_ele_ref_map;
    axis2_hash_t *simple_type_map;
    axis2_hash_t *changed_type_map;
    axis2_hash_t *processed_type_meta_info_map;
    axis2_hash_t *base_schema_type_map;
    axis2_hash_t *loaded_schema_map;
    axis2_hash_t *available_schema_map;


    /** virtual lists */
    axis2_hash_t *processed_ele_list;
    axis2_hash_t *nillable_ele_list;


} w2c_schema_compiler_impl_t;

axis2_status_t AXIS2_CALL
w2c_schema_compiler_free (w2c_schema_compiler_t *schema_compiler,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_compiler_compile( w2c_schema_compiler_t *compiler,
                const axis2_env_t *env,
                xml_schema_t *xml_schema);

axis2_status_t AXIS2_CALL
w2c_schema_compiler_compile_schema_list( w2c_schema_compiler_t *compiler,
                const axis2_env_t *env,
                axis2_array_list_t * schema_list);

axis2_status_t AXIS2_CALL
w2c_schema_compiler_get_processed_ele_map( w2c_schema_compiler_t *compiler,
                const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_compiler_get_processed_model_map( w2c_schema_compiler_t *compiler,
                const axis2_env_t *env);

/* set of private methods */

axis2_status_t AXIS2_CALL
w2c_schema_compiler_compile_inner( w2c_schema_compiler_t *compiler,
                const axis2_env_t *env,
                xml_schema_t *xml_schema,
                axis2_bool_t is_part_of_group);

axis2_status_t
w2c_schema_compiler_process_element(w2c_schema_compiler_impl_t *compiler_impl,
                        const axis2_env_t *env,
                        xml_schema_element_t *ele,
                        axis2_bool_t is_outer,
                        axis2_hash_t *inner_element_map,
                        axis2_array_list_t *local_nillable_list,
                        xml_schema_t *parent_schema);

axis2_status_t
w2c_schema_compiler_write_element(w2c_schema_compiler_impl_t *compiler_impl,
                const axis2_env_t *env,
                xml_schema_element_t *element);

axis2_status_t
w2c_schema_compiler_finalize_compilation(w2c_schema_compiler_impl_t *compiler_impl,
                const axis2_env_t *env);

axis2_char_t*
w2c_schema_compiler_find_class_name(w2c_schema_compiler_impl_t *compiler_impl,
                const axis2_env_t *env,
                axis2_qname_t *qname,
                axis2_bool_t is_array);

axis2_bool_t
w2c_schema_compiler_is_array(w2c_schema_compiler_impl_t *compiler_impl,
                const axis2_env_t *env,
                xml_schema_particle_t *particle);

xml_schema_t*
w2c_schema_compiler_resolve_parent_schema( w2c_schema_compiler_impl_t *compiler_impl,
                                const axis2_env_t *env,
                                axis2_qname_t *schema_type_qname,
                                xml_schema_t *current_schema);

axis2_qname_t*
w2c_schema_compiler_generate_type_qname(  w2c_schema_compiler_impl_t *compiler_impl,
                                        const axis2_env_t *env,
                                        axis2_qname_t *ref_ele_qname,
                                        xml_schema_t *parent_schema );

axis2_bool_t
w2c_schema_compiler_is_already_processed(  w2c_schema_compiler_impl_t *compiler_impl,
                                           const axis2_env_t *env,
                                           axis2_qname_t *qname );


axis2_char_t*
w2c_schema_compiler_find_ref_classname( w2c_schema_compiler_impl_t *compiler_impl,
                                        const axis2_env_t *env,
                                        axis2_qname_t *qname,
                                        axis2_bool_t is_array );

axis2_status_t
w2c_schema_compiler_process_schema(  w2c_schema_compiler_impl_t *compiler_impl,
                                     const axis2_env_t *env,
                                     xml_schema_element_t *ele,
                                     xml_schema_type_t *schema_type,
                                     xml_schema_t *parent_schema);

axis2_stauts_t
w2c_schema_compiler_process_anonymous_complex_schema_type( 
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    xml_schema_element_t *ele,
                                    xml_schema_complex_type_t *complex_type,
                                    xml_scheam_t *parent_schema );

axis2_status_t
w2c_schema_compiler_process_named_complex_schema_type(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    xml_schema_complex_type_t *complex_type,
                                    xml_scheam_t *parent_schema );

axis2_status_t
w2c_schema_compiler_write_complex_type(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    xml_schema_complex_type_t *complex_type,
                                    w2c_schema_writer_meta_info_t *meta_info );

w2c_schema_writer_meta_info_t*
w2c_schema_compiler_write_process_complex_type( 
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    xml_schema_complex_type_t *complex_type,
                                    xml_scheam_t *parent_schema);

axis2_status_t
w2c_schema_compiler_process_content_model(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    void *content_model,
                                    w2c_schema_writer_meta_info_t *meta_info, 
                                    xml_scheam_t *parent_schema);

axis2_status_t
w2c_schema_compiler_process_complex_content(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    xml_schema_complex_content_t *complex_content,
                                    w2c_schema_writer_meta_info_t *meta_info, 
                                    xml_scheam_t *parent_schema);

axis2_status_t
w2c_schema_compiler_copy_meta_info(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    w2c_schema_writer_meta_info_t *meta_info_child,
                                    axis2_qname_t *base_type_qname,
                                    xml_schema_t *parent_schema );

axis2_status_t
w2c_schema_compiler_process_simple_content(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    w2c_schema_simple_content_t *simple_content,
                                    w2c_schema_writer_meta_info_t *meta_info,
                                    xml_schema_t *parent_schema );

axis2_status_t
w2c_schema_compiler_process_simple_extension_base_type(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    axis2_qname_t *ext_base_type,
                                    w2c_schema_writer_meta_info_t *meta_info);

axis2_status_t
w2c_schema_compiler_process_simple_restriction_base_type(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    axis2_qname_t *res_base_type,
                                    w2c_schema_writer_meta_info_t *meta_info);

axis2_status_t
w2c_schema_compiler_process_facets(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    xml_schema_obj_collection_t *facets,
                                    w2c_schema_writer_meta_info_t *meta_info);

axis2_status_t
w2c_schema_compiler_process_any_attributes(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    w2c_schema_writer_meta_info_t *meta_info,
                                    xml_schema_any_attribute_t *any_attri );

axis2_status_t
w2c_schema_compiler_process_attribute(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    w2c_schema_writer_meta_info_t *meta_info,
                                    xml_schema_attribute_t *attri );

axis2_status_t
w2c_schema_compiler_process_particle(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    xml_schema_particle_t *particle,
                                    w2c_schema_writer_meta_info_t *meta_info,
                                    xml_schema_t *parent_schema );
axis2_status_t
w2c_schema_compiler_process( w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    xml_schema_obj_collection_t *items,
                                    w2c_schema_writer_meta_info_t *meta_info,
                                    axis2_bool_t order,
                                    xml_schema_t *parent_schema );

axis2_bool_t
w2c_schema_compiler_is_binary(  w2c_schema_compiler_impl_t *compiler_impl,
                                const axis2_env_t *env,
                                xml_schema_element_t *ele );

axis2_status_t
w2c_schema_compiler_process( w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    xml_schema_simple_type_t *simple_type,
                                    xml_scheme_element_t *ele );


AXIS2_EXTERN w2c_schema_compiler_t * AXIS2_CALL
w2c_schema_compiler_create( const axis2_env_t *env,
        w2c_schema_compiler_options_t *compiler_options)
{
    w2c_schema_compiler_impl_t *compiler_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    compiler_impl = (w2c_schema_compiler_impl_t *) AXIS2_MALLOC(env->
               allocator, sizeof(w2c_schema_compiler_impl_t));

    if(NULL == compiler_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    compiler_impl->compiler_options = compiler_options;
    /* creates empty hashes */
    compiler_impl->processed_type_map = axis2_hash_make( env);
    compiler_impl->processed_ele_map = axis2_hash_make( env);
    compiler_impl->simple_type_map = axis2_hash_make( env);
    compiler_impl->processed_anonymous_complex_type_map = axis2_hash_make( env);
    compiler_impl->changed_type_map = axis2_hash_make( env);
    compiler_impl->processed_type_meta_info_map = axis2_hash_make( env);
    compiler_impl->processed_ele_ref_map = axis2_hash_make( env);
    compiler_impl->loaded_schemap_map = axis2_hash_make( env);

    compiler_impl->processed_ele_list = axis2_hash_make( env);
    compiler_impl->nillable_ele_list = axis2_hash_make( env);

    compiler_impl->compiler.ops =
    AXIS2_MALLOC( env->allocator, sizeof(w2c_schema_compiler_ops_t));
    if(NULL == compiler_impl->compiler.ops)
    {
        w2c_schema_compiler_free(&(compiler_impl->compiler), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    compiler_impl->compiler.ops->free = w2c_schema_compiler_free;
    compiler_impl->compiler.ops->compile = w2c_schema_compiler_compile;
   
    return &(compiler_impl->compiler);
}

axis2_status_t AXIS2_CALL
w2c_schema_compiler_free (w2c_schema_compiler_t *schema_compiler,
                            const axis2_env_t *env)
{
    w2c_schema_compiler_impl_t *schema_compiler_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    schema_compiler_impl = W2C_INTF_TO_IMPL(schema_compiler);

    if( schema_compiler_impl-> compiler_options)
    {
        W2C_SCHEMA_COMPILER_OPTION_FREE(schema_compiler_impl-> compiler_options, env);
    }
    if( schema_compiler_impl-> writer)
    {
        W2C_SCHEMA_WRITER_FREE( schema_compiler_impl-> writer);
    }
    if( schema_compiler_impl-> processed_type_map)
    {
        axis2_hash_free( schema_compiler_impl-> processed_type_map, env);
    }
    if( schema_compiler_impl-> processed_ele_map)
    {
        axis2_hash_free( schema_compiler_impl-> processed_ele_map, env);
    }
    if( schema_compiler_impl-> loaded_schema_map)
    {
        axis2_hash_free( schema_compiler_impl-> loaded_schema_map, env);
    }
    if( schema_compiler_impl-> processed_anonymous_complex_typemap)
    {
        axis2_hash_free( schema_compiler_impl-> processed_anonymous_complex_typemap, env);
    }
    if( schema_compiler_impl-> processed_ele_ref_map)
    {
        axis2_hash_free( schema_compiler_impl-> processed_ele_ref_map, env);
    }
    if( schema_compiler_impl-> simple_type_map)
    {
        axis2_hash_free( schema_compiler_impl-> simple_type_map, env);
    }
    if( schema_compiler_impl-> changed_type_map)
    {
        axis2_hash_free( schema_compiler_impl-> changed_type_map, env);
    }
    if( schema_compiler_impl-> processed_type_meta_info_map)
    {
        axis2_hash_free( schema_compiler_impl-> processed_type_meta_info_map, env);
    }
    if( schema_compiler_impl-> base_schema_type_map)
    {
        axis2_hash_free( schema_compiler_impl-> base_schema_type_map, env);
    }
    if( schema_compiler_impl-> loaded_schema_map)
    {
        axis2_hash_free( schema_compiler_impl-> loaded_schema_map, env);
    }
    if( schema_compiler_impl-> available_schema_map)
    {
        axis2_hash_free( schema_compiler_impl-> available_schema_map, env);
    }
    if( schema_compiler_impl-> processed_ele_list)
    {
        axis2_hash_free( schema_compiler_impl-> processed_ele_list, env);
    }
    if( schema_compiler_impl-> nillable_ele_list)
    {
        axis2_hash_free( schema_compiler_impl-> nillable_ele_list, env);
    }

   
    if(schema_compiler->ops)
    {
        AXIS2_FREE(env->allocator, schema_compiler->ops);
        schema_compiler->ops = NULL;
    }
    if(schema_compiler_impl)
    {
        AXIS2_FREE(env->allocator, schema_compiler_impl);
        schema_compiler_impl = NULL;
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_schema_compiler_compile_inner( w2c_schema_compiler_t *compiler,
                const axis2_env_t *env,
                xml_schema_t *xml_schema,
                axis2_bool_t is_part_of_group)
{
    w2c_schema_compiler_t *compiler__impl = NULL;
    axis2_char_t *target_ns = NULL;
    xml_schema_t *schema_stored = NULL;
    xml_schema_obj_collection_t *includes = NULL;
    int i = 0, count = 0;
    xml_schema_obj_t *schema_obj = NULL;
    xml_schema_type_t *schema_type = NULL;
    xml_schema_import_t *schema_import = NULL;
    xml_schema_t *schema1 = NULL;
    xml_schema_external_t *schema1_external = NULL;
    xml_schema_obj_table_t *elements = NULL;
    axis2_array_list_t *element_value_list = NULL;
    xml_schema_element_t *element = NULL;
    axis2_char_t *package_name = NULL;


    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    compiler_impl = W2C_INTF_TO_IMPL(compiler);
    /**
     * some documents explicitly imports the schema of built in types. 
     * We don't actually need to compile the built-in types.
     * So when importing check the target namespace here and ignore it.
     */
    target_ns = XML_SCHEMA_GET_TARGET_NAMESPACE( xml_schema, env);
    if ( !STRCMP(target_ns, AXIS2_URI_2001_SCHEMA_XSD) )
    {
        return AXIS2_SUCCESS;
    }

    /**
     * the following is set for extend this to languages like java */
    if( !is_part_of_group)
    {
        package_name = W2C_SCHEMA_WRITER_GET_MAPPER_PACKAGE_NAME(writer, env);
        if ( NULL == package_name )
        {
            /** derive package name from the namespace */
            pacakge_name = w2c_url_processor_make_package_name(env, target_ns);
            W2C_SCHEMA_WRITER_SET_MAPPER_PACKAGE_NAME( package_name, env);
        }
    }

    /** adding the schema to loaded schema map */
    schema_stored = axis2_hash_get( compiler_impl-> loaded_schema_map, target_ns,
            AXIS2_HASH_KEY_STRING );
    if( NULL == schema_stored)
    {
        axis2_hash_put( compiler_impl-> loaded_schema_map, target_ns,
                AXIS2_HASH_KEY_STRING, schema);
    }

    /** compile though all the imports and includes */
    includes = XML_SCHEMA_GET_INCLUDES( xml_schema, env);
    count = XML_SCHEMA_OBJ_COLLECTION_GET_COUNT( includes, env);
    for ( i = 0; i < count; i ++)
    {
        schema_obj = XML_SCHEMA_OBJ_COLLECTION_GET_ITEM( inclues, env, i );
        schema_type = XML_SCHEMA_OBJ_GET_SCHEMA_TYPE( schema_obj, env);
        if ( XML_SCHEMA_OBJ_GET_TYPE( schema_obj, env) == XML_SCHEMA_IMPORT )
        {
            schema_import = (xml_schema_import_t*)schema_obj;
            schema_external = XML_SCHEMA_IMPORT_GET_BASE_IMPL( schema_import, env);
            if( schema_external) 
                schema1 = XML_SCHEMA_EXTERNAL_GET_SCHEMA( schema_external, env);
            if (schema1)
            {
                w2c_schema_compiler_compile_inner( compiler, env, schema1, is_part_of_group);
            }
        }
        /** TODO: update here when XML_SCHEMA_INCLUDE is ready */
    }
    /*select all the elements. We generate the code for types
     *only if the elements refer them!!! regardless of the fact that
     *we have a list of elementnames, we'll need to process all the elements
     */
    elements = XML_SCHEMA_GET_ELEMENTS( schema, env);
    element_value_list = XML_SCHEMA_OBJ_TABLE_GET_VALUES( elements, env);
    count = AXIS2_ARRAY_LIST_SIZE( element_value_list, env);
    for (i = 0; i < count; i ++ )
    {
        element = AXIS2_ARRAY_LIST_GET( element_value_list, env, i );
        /*this is the set of outer elements so we need to generate classes
         *The outermost elements do not contain occurence counts (!) so we do not need
         *to check for arraytypes
         */
        xml_schema_compiler_process_element( compiler_impl, env, element, schema);
    }

    /* re-iterate through the elements and write them one by one
     * if the mode is unpack this process will not really write the
     * classes but will accumilate the models for a final single shot
     * write
     */
    for (i = 0; i < count; i ++ )
    {
        element = AXIS2_ARRAY_LIST_GET( element_value_list, env, i );
        /* this is the set of outer elements so we need to generate classes */
        xml_schema_compiler_write_element( compiler_impl, env, element);
    }

    if (!is_part_of_group){
        /* complete the compilation */
        w2c_schema_compiler_finalize_compilation( compiler_impl, env);
    }

}

axis2_status_t
w2c_schema_compiler_finalize_compilation(w2c_schema_compiler_impl_t *compiler_impl,
                        const axis2_env_t *env)
{
    W2C_SCHEMA_WRITER_WRITE_EXTENSION_MAPPER( compiler_impl-> writer, 
            env, processed_type_meta_info_map );
    if( W2C_SCHEMA_COMPILER_OPTIONS_IS_WRAP_CLASSES( compiler_impl-> options, env) )
    {
        W2C_SCHEMA_WRITER_WRITE_BATCH( compiler_impl-> writer, env);
    }
    return AXIS2_FAILURE;
}

axis2_status_t
w2c_schema_compiler_write_element(w2c_schema_compiler_impl_t *compiler_impl,
                const axis2_env_t *env,
                xml_schema_element_t *element)
{
    axis2_qname_t *qname = NULL;
    axis2_char_t *qname_str = NULL;
    void *existing = NULL;
    xml_schema_type_t *schema_type = NULL;
    w2c_schema_writer_meta_info_t *meta_inf = NULL;
    axis2_char_t *class_name = NULL;
    axis2_char_t *qname2str = NULL;
    axis2_hash_t *base_hash = NULL;
    xml_schema_particle_t *particle = NULL;
    axis2_bool_t is_array = NULL;
    axis2_qname_t *type_qname = NULL;
    axis2_char_t *type_name = NULL;
    axis2_qname_t *changed_qname = NULL;
    axis2_qname_t *element_type_qname = NULL;
    axis2_char_t *written_classname = NULL;

    qname = XML_SCHEMA_ELEMENT_GET_QNAME( element, env);
    qname_str = AXIS2_QNAME_TO_STRING( qname, env);
    
    existing = axis2_hash_get( processed_element_map, qname_str, AXIS2_HASH_KEY_STRING);
    if ( existing != NULL )
    {
        return AXIS2_SUCCESS;
    }
    meta_inf = w2c_schema_writer_meta_info_create( env);
    base_hash = XML_SCHEMA_ELEMENT_SUPER_OBJS( element, env);
    particle = axis2_hash_get( base_hash, "XML_SCHEMA_PARTICLE", AXIS2_HASH_KEY_STRING);
    is_array = w2c_schema_compiler_is_array( compiler_impl, env, particle);

    schema_type = XML_SCHEMA_ELEMENT_GET_SCHEMA_TYPE( element, env);
    if( schema_type != NULL )
    {
        type_name = XML_SCHEMA_TYPE_GET_NAME(schema_type, env);
    }
    if( type_name != NULL)
    {
        type_qname = XML_SCHEMA_TYPE_GET_QNAME(schema_type, env);
        class_name = w2c_schema_compiler_find_class_name( compiler_impl, env, qname, is_array);
        qname2str = AXIS2_QNAME_TO_STRING( type_qname, env);
        changed_qname = axis2_hash_get( changed_type_map, qname2str, AXIS2_HASH_KEY_STRING);
        /** this means the schema type actually returns a different QName */
        if ( changed_qname == NULL )
        {
            changed_qname = qname;
        }
        W2C_SCHEMA_WRITER_META_INFO_REGISTER_MAP(meta_inf, env, 
                               qname, changed_qname, class_name,
                               W2C_SCHEMA_CONSTS_NULL);
    }
    else
    {
        element_type_qname = 
            XML_SCHEMA_ELEMENT_GET_SCHEMA_TYPE_QNAME( element, env);
        if (element_type_qname != NULL)
        {
            class_name = w2c_schema_compiler_find_class_name( compiler_impl, env,
                               element_type_qname, is_array );
            W2C_SCHEMA_WRITER_META_INFO_REGISTER_MAP(meta_inf, env,
                               qname, element_type_qname, class_name,
                               W2C_SCHEMA_CONSTS_NULL);
        }
        else /* unnamed types would be handled here */
        {
            if ( schema_type != NULL )
            {
                /* TODO: recheck getting the meta inf corrosponding to the element */
                meta_inf = axis2_hash_get(compiler_impl-> processed_anonymous_complex_typemap,
                                         qname_str, AXIS2_HASH_KEY_STRING);
                W2C_SCHEMA_WRITER_META_INFO_SET_ANONYMOUS( meta_inf, env, AXIS2_TRUE);
            }
            else
            {
                /** TODO: do some error logging here */
                return AXIS2_FAILURE;
            }
        }
    }

    if ( axis2_hash_get( compiler_impl-> nillable_ele_list,
                                 qname_str, AXIS2_HASH_KEY_STRING) )
    {
        W2C_SCHEMA_WRITER_META_INFO_REGISTER_NILLABLE( qname, env);
    }

    written_classname = 
        W2C_SCHEMA_WRITER_WRITE( element, 
                compiler_impl->processed_type_map, metainf);
    axis2_hash_set( compiler_impl->processed_ele_map, qname_str,
            AXIS2_HASH_KEY_STRING, written_classname );

    /** registering the class is not done */
    return AXIS2_SUCCESS;
}


axis2_char_t*
w2c_schema_compiler_find_class_name(w2c_schema_compiler_impl_t *compiler_impl,
                const axis2_env_t *env,
                axis2_qname_t *qname,
                axis2_bool_t is_array)
{
    axis2_char_t *qname_str = NULL;
    axis2_char_t *class_name = NULL;

    qname_str = AXIS2_QNAME_TO_STRING( qname, env);
    class_name = axis2_hash_get( processed_element_map, qname_str,
                                AXIS2_HASH_KEY_STRING);
    if ( NULL == class_name )
    {
        class_name = axis2_hash_get( simple_type_map, qname_str,
                                AXIS2_HASH_KEY_STRING);
    }
    if ( NULL == class_name )
    {
        class_name = axis2_hash_get( base_schema_type_map, qname_str,
                                AXIS2_HASH_KEY_STRING);
    }
    if ( NULL == class_name )
    {
        qname = W2C_TYPEMAPPER_GET_DEFAULT_QNAME( simple_type_mapper, env);
        class_name= W2C_TYPEMAPPER_GET_TYPE_NAME( simple_type_mapper, env, qname);
    }
    if ( is_array )
    {
        /** TODO: make class name for arrays in language independent way */
    }
    return class_name;
}

axis2_bool_t
w2c_schema_compiler_is_array(w2c_schema_compiler_impl_t *compiler_impl,
                const axis2_env_t *env,
                xml_schema_particle_t *particle)
{
    int min_occurs = 0;
    int max_occurs = 0;

    min_occurs = XML_SCHEMA_PARTICLE_GET_MIN_OCCURS( particle, env);
    max_occurs = XML_SCHEMA_PARTICLE_GET_MAX_OCCURS( particle, env);

    if ( min_occurs > max_occurs )
    {
        return AXIS2_FALSE;
    }
    else
    {
        return max_occurs > 1;
    }
}

axis2_status_t
w2c_schema_compiler_process_element(w2c_schema_compiler_impl_t *compiler_impl,
                        const axis2_env_t *env,
                        xml_schema_element_t *ele,
                        axis2_bool_t is_outer,
                        axis2_hash_t *inner_element_map,
                        axis2_array_list_t *local_nillable_list,
                        xml_schema_t *parent_schema)
{
    int processed = NULL;
    axis2_qname_t *qname = NULL;
    axis2_char_t *qname_str = NULL;
    xml_schema_type_t *schema_type = NULL;
    axis2_char_t *class_name = NULL;

    axis2_hash_t *base_hash = NULL;
    xml_schema_particle_t *particle = NULL;
    axis2_bool_t is_array = NULL;

    axis2_qname_t *type_qname = NULL;
    axis2_char_t *type_name = NULL;
    axis2_qname_t *generated_type_qname = NULL;
    w2c_schema_writer_meta_info_t *meta_inf = NULL;
    xml_schema_complex_type_t *complex_type = NULL;
    
    axis2_qname_t *ref_qname = NULL;
    xml_schema_t *current_parent_schema = NULL;
    xml_schema_element_t *ref_ele = NULL;
    axis2_qname_t *ref_ele_qname = NULL;
    xml_schema_type_t *ref_schema_type = NULL;

    axis2_qname_t *schema_type_name = NULL;
    xml_schema_type_t *type_by_name = NULL;


    /** param check is done with displaying the message */
    if( NULL == ele )
    {
        w2c_messages_print_n_log_error( env, "schema.elementNull");
        return AXIS2_FAILURE;
    }
    qname = XML_SCHEMA_ELEMENT_GET_QNAME( ele, env);
    qname_str = AXIS2_QNAME_TO_STRING( qname, env);
    processed = (int)axis2_hash_get( compiler_impl-> processed_ele_list, qname_str,
                                  AXIS2_HASH_KEY_STRING );
    if ( is_outer && processed != 0 )
    {
        /* this is already processed */
        return;
    }
    base_hash = XML_SCHEMA_ELEMENT_SUPER_OBJS( ele, env);
    particle = axis2_hash_get( base_hash, "XML_SCHEMA_PARTICLE", AXIS2_HASH_KEY_STRING);
    is_array = w2c_schema_compiler_is_array( compiler_impl, env, particle);

    schema_type = XML_SCHEMA_GET_SCHEMA_TYPE( ele, env);
    ref_qname = XML_SCHEMA_ELEMENT_GET_REF_NAME( ele, env);
    schema_type_qname = XML_SCHEMA_ELEMENT_GET_SCHEMA_TYPE_QNAME( ele, env);
    if( schema_type != NULL)
    {
        w2c_schema_compiler_process_schema( compiler_impl, env, schema_type, ele, parent_schema);
        /** currently we put every thing to an arraylist and later they will be iterated to write */

        if ( !is_outer)
        {
            type_qname = XML_SCHEMA_TYPE_GET_QNAME( schema_type, env);
            type_name = XML_SCHEMA_TYPE_GET_NAME( schema_type, env);
            if ( type_name != NULL)
            {
                qname_str  = AXIS2_QNAME_TO_STRING( type_qname, env);
                class_name = w2c_schema_compiler_find_class_name( compiler_impl, env, type_qname, is_array);
                axis2_hash_set( compiler_impl-> inner_ele_map, qname_str, AXIS2_HASH_KEY_STRING, class_name);
                /** not adding meta info to schema_type */
            }
            else
            {
                /** handling anonymous types */
                generated_type_qname = w2c_schema_compiler_generate_type_qname( compiler_impl, env, qname, parent_schema);
                if ( XML_SCHEMA_COMPLEX_TYPE == XML_SCHEMA_TYPE_GET_TYPE( schema_type, env) )
                {
                    local_part = AXIS2_QNAME_GET_LOCALPART( generated_type_qname, env);
                    XML_SCHEMA_TYPE_SET_NAME( schema_type, env, local_part);
                    qname_str  = AXIS2_QNAME_TO_STRING( qname, env);
                    meta_inf = axis2_hash_get(compiler_impl-> processed_anonymous_complex_typemap,
                                                                 qname_str, AXIS2_HASH_KEY_STRING);

                    /** remove this from anonymous list */
                    axis2_hash_set( compiler_impl-> processed_anonymous_complex_typemap, 
                                                qname_str, AXIS2_HASH_KEY_STRING, 0 );

                    base_hash = XML_SCHEMA_TYPE_SUPER_OBJS( schema_type, env);
                    complex_type = axis2_hash_get( base_hash, "XML_SCHEMA_COMPLEX_TYPE", AXIS2_HASH_KEY_STRING);

                    w2c_schema_compiler_write_complex_type( complex_type, meta_inf);
                    
                    type_qname = XML_SCHEMA_TYPE_GET_QNAME( schema_type, env);
                    qname_str  = AXIS2_QNAME_TO_STRING( type_qname, env);
                    class_name = w2c_schema_compiler_find_class_name( compiler_impl, env, type_qname, is_array);
                    axis2_hash_set( compiler_impl-> inner_ele_map, qname_str, AXIS2_HASH_KEY_STRING, class_name);
                    /** not adding meta info to schema_type */
                }
            }
        }
        else
        {
            qname_str  = AXIS2_QNAME_TO_STRING( qname, env);
            axis2_hash_set( compiler_impl-> processed_ele_list, qname_str, AXIS2_HASH_KEY_STRING, 1);
        }
    }
    else if ( ref_qname != NULL )
    {
        current_parent_schema = w2c_schema_compiler_resolve_parent_schema( compiler_impl, env, ref_qname, parent_schema);
        ref_ele = XML_SCHEMA_GET_ELEMENT_BY_QNAME( current_parent_schema, env, ref_qname);
        /** refered element should be an outer one */
        w2c_schema_compiler_process_element( ref_ele, parent_schema);

        ref_ele_qname = XML_SCHEMA_ELEMENT_GET_QNAME( ref_ele, env);
        type_qname = XML_SCHEMA_ELEMENT_GET_SCHEMA_TYPE_QNAME( ref_ele, env);
        type_name = AXIS2_QNAME_GET_LOCALPART( type_qname, env);
        if( NULL != type_name )
        {
            class_name = w2c_schema_compiler_find_class_name( compiler_impl, env, type_qname, is_array);
            qname_str  = AXIS2_QNAME_TO_STRING( ref_ele_qname, env);
            axis2_hash_set( compiler_impl-> processed_ele_ref_map, qname_str, AXIS2_HASH_KEY_STRING, class_name);
            /** ignore adding meta info */
        }
        else
        {
            /** handling anonymous reference elements */
            generated_type_qname = w2c_schema_compiler_generate_type_qname( compiler_impl, env, ref_ele_qname, parent_schema);
            ref_schema_type = XML_SCHEMA_ELEMENT_GET_SCHEMA_TYPE( ref_ele, env);
            if ( XML_SCHEMA_COMPLEX_TYPE == XML_SCHEMA_TYPE_GET_TYPE( ref_schema_type, env) )
            {
                local_part = AXIS2_QNAME_GET_LOCALPART( generated_type_qname, env);
                XML_SCHEMA_TYPE_SET_NAME( ref_schema_type, env, local_part);
                meta_inf = axis2_hash_get(compiler_impl-> processed_anonymous_complex_typemap,
                                                             qname_str, AXIS2_HASH_KEY_STRING);
_
                /** remove this from anonymous list */
                qname_str  = AXIS2_QNAME_TO_STRING( ref_ele_qname, env);
                axis2_hash_set( compiler_impl-> processed_anonymous_complex_typemap, 
                                            qname_str, AXIS2_HASH_KEY_STRING, 0 );

                base_hash = XML_SCHEMA_TYPE_SUPER_OBJS( ref_schema_type, env);
                complex_type = axis2_hash_get( base_hash, "XML_SCHEMA_COMPLEX_TYPE", AXIS2_HASH_KEY_STRING);

                w2c_schema_compiler_write_complex_type( complex_type, meta_inf);
                
                type_qname = XML_SCHEMA_TYPE_GET_QNAME( ref_schema_type, env);
                qname_str  = AXIS2_QNAME_TO_STRING( type_qname, env);
                class_name = w2c_schema_compiler_find_class_name( compiler_impl, env, type_qname, is_array);
                axis2_hash_set( compiler_impl-> processed_ele_ref_map, qname_str, AXIS2_HASH_KEY_STRING, class_name);
                /** not adding meta info to ref_schema_type */
            }
        }
    }
    else if( schema_type_qname != NULL )
    {
        current_parent_schema = w2c_schema_compiler_resolve_parent_schema( compiler_impl, env, schema_type_qname, parent_schema);
        type_by_name = XML_SCHEMA_GET_TYPE_BY_QNAME( current_parent_schema, env, schema_type_qname);
        if (NULL != type_by_name )
        {
            /*this type is found in the schema so we can process it*/
            w2c_schema_compiler_process_schema( ele, type_by_name, current_parent_schema);
            qname_str = AXIS2_QNAME_TO_STRING( qname, env);
            if( !is_outer)
            {
                class_name = w2c_schema_compiler_find_class_name( compiler_impl,
                                 env, schema_type_qname, is_array);
                axis2_hash_set( compiler_impl-> inner_ele_map, qname_str, AXIS2_HASH_KEY_STRING, class_name);
            }
            else
            {
                axis2_hash_set( compiler_impl->processed_ele_list, qname_str,
                    AXIS2_HASH_KEY_STRING, (void*)1);
            }
        }
        else
        {
            /* this type is not found at all. we'll just register it with 
             * whatever the class name we can comeup with */
            if( !is_outer)
            {
                class_name = w2c_schema_compiler_find_class_name( compiler_impl,
                                 env, schema_type_qname, is_array);
                axis2_hash_set( compiler_impl-> inner_ele_map, qname_str, AXIS2_HASH_KEY_STRING, class_name);
            }
            else
            {
                axis2_hash_set( compiler_impl->processed_ele_list, qname_str,
                    AXIS2_HASH_KEY_STRING, (void*)1);
            }
        }
    }

    /* add this elements QName to the nillable group if it has the  nillable attribute */
    if( XML_SCHEMA_ELEMENT_IS_NILLABLE( ele, env) )
    {
        if( is_outer)
        {
            axis2_hash_get( compiler_impl-> nillable_ele_list,
                                 qname_str, AXIS2_HASH_KEY_STRING, (void*)1);
        }
        else
        {
            axis2_hash_get( compiler_impl-> local_nillable_ele_list,
                                 qname_str, AXIS2_HASH_KEY_STRING, (void*)1);
        }
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_schema_compiler_compile_schema_list( w2c_schema_compiler_t *compiler,
                const axis2_env_t *env,
                axis2_array_list_t * schema_list)
{
    w2c_schema_complier_impl_t *compiler_impl = NULL;
    int size = 0;
    int i = 0;
    xml_schema_t *schema = NULL;
    axis2_char_t *ns = NULL;
    axis2_char_t *mapper_package_name = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    compiler_impl = W2C_INTF_TO_IMPL(compiler);

    if( AXIS2_ARRAY_LIST_IS_EMPTY( schema_list, env) )
    {
        return AXIS2_SUCCESS;
    }
    /*TODO: clear the loaded and available maps */
    size = AXIS2_ARRAY_LIST_SIZE( schema_list, env);
    /* first round - populate the avaialble map */
    for( i = 0; i < size; i ++ )
    {
        schema = (xml_schema_t*) AXIS2_ARRAY_LIST_GET( schema_list, env, i);
        ns = XML_SCHEMA_GET_TARGET_NAMESPACE( schema, env);
        axis2_hash_set( compiler_impl-> available_schema_map, ns,
                AXIS2_HASH_KEY_STRING, schema);
        if( 0 == i )
        {
            mapper_package_name = w2c_url_processor_make_package_name(env, ns);
        }
    }
    /* set a mapper package if not avaialable */
    if( NULL == XML_SCHEMA_WRITER_GET_EXTENSION_MAPPER_PACKAGE_NAME( writer, env))
    {
        XML_SCHEMA_WRITER_GET_EXTENSION_MAPPER_PACKAGE_NAME( writer, env, 
                mapper_package_name);
    }
    /*  second round - call the schema compiler one by one */
    for( i = 0; i < size; i ++ )
    {
        schema = (xml_schema_t*) AXIS2_ARRAY_LIST_GET( schema_list, env, i);
        w2c_schema_compiler_compile_inner( compiler, env, schema, env, AXIS2_TRUE);
    }

    /* finalize only once for all */
    w2c_schema_compiler_finalize_schema_compilation( compiler_impl, env);

    return AXIS2_SUCCESS;
}

xml_schema_t*
w2c_schema_compiler_resolve_parent_schema( w2c_schema_compiler_impl_t *compiler_impl,
                                const axis2_env_t *env,
                                axis2_qname_t *schema_type_qname,
                                xml_schema_t *current_schema)
{
    axis2_char_t *target_ns = NULL;
    xml_schema_t *loaded_schema = NULL;
    xml_schema_t *schema = NULL;
    
    target_ns = AXIS2_QNAME_GET_URI( schema_type_qname, env);
    loaded_schema = (xml_schema_t*)axis2_hash_get( compiler_impl-> loaded_schema_map,
            target_ns, AXIS2_HASH_KEY_STRING );
    if ( loaded_schema != NULL )
    {
        return loaded_schema;
    }
    schema = (xml_schema_t*)axis2_hash_get( compiler_impl-> available_schema_map,
            target_ns, AXIS2_HASH_KEY_STRING );
    if ( schema != NULL )
    {
        w2c_schema_compiler_compile_inner( compiler, env, schema, AXIS2_FALSE);
        return schema;
    }
    /* otherwise default */
    return current_schema;
}

axis2_qname_t*
w2c_schema_compiler_generate_type_qname(  w2c_schema_compiler_impl_t *compiler_impl,
                                        const axis2_env_t *env,
                                        axis2_qname_t *ref_ele_qname,
                                        xml_schema_t *parent_schema )
{
    axis2_char_t *local_part = NULL;
    axis2_char_t *ns_uri = NULL;
    axis2_char_t *new_local_part = NULL;
    static int counter = 0;
    axis2_char_t *counter_str = NULL;
    axis2_qname_t *qname = NULL;

    ns_uri = AXIS2_QNAME_GET_URI( ref_ele_qname, env);
    local_part = AXIS2_QNAME_GET_LOCALPART( ref_ele_qname, env);

    while( AXIS2_TRUE)
    {
        sprintf( counter_str, "%d", counter);
        new_local_part = axis2_stracat( local_part, counter_str, env);
        qname = axis2_qname_create( env, new_local_part, ns_uri, NULL);
        if ( NULL == XML_SCHEMA_GET_TYPE_BY_QNAME( parent_schema, env, qname ) )
        {
            /* new suitable qname found */
            return qname;
        }
        /* else */
        AXIS2_QNAME_FREE( qname);
    }
}

axis2_bool_t
w2c_schema_compiler_is_already_processed(  w2c_schema_compiler_impl_t *compiler_impl,
                                           const axis2_env_t *env,
                                           axis2_qname_t *qname )
{
    axis2_char_t *qname_str = AXIS2_QNAME_TO_STRING( qname );
    return axis2_hash_get( compiler_impl-> processed_type_map, qname_str, AXIS2_HASH_KEY_STRING ) ||
           axis2_hash_get( compiler_impl-> simple_type_map, qname_str, AXIS2_HASH_KEY_STRING ) ||
           axis2_hash_get( compiler_impl-> base_schema_type_map, qname_str, AXIS2_HASH_KEY_STRING ); 
}

axis2_char_t*
w2c_schema_compiler_find_ref_classname( w2c_schema_compiler_impl_t *compiler_impl,
                                        const axis2_env_t *env,
                                        axis2_qname_t *qname,
                                        axis2_bool_t is_array )
{
    axis2_char_t *qname_str = NULL;
    axis2_char_t *class_name = NULL;

    qname_str = AXIS2_QNAME_TO_STRING( qname_str, env);
    class_name = (axis2_char_t*)
        axis2_hash_get( compiler_impl-> processed_ele_ref_map, qname_str, AXIS2_HASH_KEY_STRING);
    if ( is_array)
    {
        /* TODO: do something in language independent manner */
        /* not applicable to c */
    }
    return class_name;
}

axis2_status_t
w2c_schema_compiler_process_schema(  w2c_schema_compiler_impl_t *compiler_impl,
                                     const axis2_env_t *env,
                                     xml_schema_element_t *ele,
                                     xml_schema_type_t *schema_type,
                                     xml_schema_t *parent_schema)
{
    xml_schema_complex_type_t *complex_type = NULL;
    xml_schema_simple_type_t *simple_type = NULL;
    axis2_hash_t *base_hash = NULL;

    if( XML_SCHEMA_COMPLEX_TYPE == XML_SCHEMA_TYPE_GET_TYPE( schema_type, env) )
    {
        base_hash = XML_SCHEMA_TYPE_SUPER_OBJS( schema_type, env);
        complex_type = axis2_hash_get( base_hash, "XML_SCHEMA_COMPLEX_TYPE", AXIS2_HASH_KEY_STRING);
        if ( XML_SCHEMA_TYPE_GET_NAME( schema_type, env ) != NULL )
        {
            w2c_schema_compiler_process_named_complex_schema_type(
                    compler_impl, env, complex_type, parent_schema);
        }
        else
        {
            w2c_schema_compiler_process_anonymous_complex_schema_type( 
                    compler_impl, env, ele, complex_type, parent_schema);
        }
    }
    else if ( XML_SCHEMA_SIMPLE_TYPE == XML_SCHEMA_TYPE_GET_TYPE( schema_type, env) )
    {
        base_hash = XML_SCHEMA_TYPE_SUPER_OBJS( schema_type, env);
        simple_type = axis2_hash_get( base_hash, "XML_SCHEMA_SIMPLE_TYPE", AXIS2_HASH_KEY_STRING);
        w2c_schema_compiler_process_schema_type( compler_impl, env, simple_type, ele );
    }
    return AXIS2_SUCCESS;
}

axis2_stauts_t
w2c_schema_compiler_process_anonymous_complex_schema_type( 
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    xml_schema_element_t *ele,
                                    xml_schema_complex_type_t *complex_type,
                                    xml_scheam_t *parent_schema )
{
    w2c_schema_writer_meta_info_t *meta_info = NULL;
    axis2_qname_t *qname = NULL;
    axis2_char_t *qname_str = NULL;

    meta_info = w2c_schema_compiler_process_complex_type( complex_type, parent_schema);
    qnmae = XML_SCHEMA_ELEMENT_GET_QNAME( ele, env);
    qname_str = AXIS2_QNAME_TO_STRING( qname_str, env);

    axis2_hash_set( compiler_impl-> processed_anonymous_complex_typemap, qname_str, 
                    AXIS2_HASH_KEY_STRING, meta_info);
    return AXIS2_SUCCESS;
}

axis2_status_t
w2c_schema_compiler_process_named_complex_schema_type(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    xml_schema_complex_type_t *complex_type,
                                    xml_scheam_t *parent_schema )
{

    axis2_hash_t *base_hash = NULL;
    xml_schema_type_t *schema_type = NULL;
    axis2_qname_t *qname = NULL;
    axis2_char_t *qname_str = NULL;
    axis2_char_t *fully_qualified_class_name = NULL;
    w2c_schema_writer_meta_info_t *meta_info = NULL;

    base_hash = XML_SCHEMA_COMPLEX_TYPE_SUPER_OBJS( complex_type, env);
    schema_type = axis2_hash_get( base_hash, "XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING);
    qname = XML_SCHEMA_TYPE_GET_QNAME( schema_type, env);
    qname_str = AXIS2_QNAME_TO_STRING( qname, env);

    if( axis2_hash_get( compiler_impl-> processed_typemap, qname_str, AXIS2_HASH_KEY_STRING) ||
        axis2_hash_get( compiler_impl-> base_schema_typemap, qname_str, AXIS2_HASH_KEY_STRING) )
    {
        return AXIS2_SUCCESS;
    } 

    /* Must do this up front to support recursive types */
    fully_qualified_class_name = W2C_SCHEMA_WRITER_MAKE_FULLY_QUALIFIED_PATH( 
                     compiler_impl-> writer, qname_str );
    axis2_hash_set( compiler_impl-> processed_type_map, qname_str,
                   AXIS2_HASH_KEY_STRING, fully_qualified_class_name );
    
    /* ignoring adding meta info */
    meta_info = w2c_schema_compiler_process_complex_type( complex_type, parent_schema);
    W2C_SCHEMA_WRITER_META_INFO_SET_OWN_QNAME( meta_info, env, qname);
    W2C_SCHEMA_WRITER_META_INFO_SET_OWN_CLASS_NAME( meta_info, env, 
                                            fully_qualified_class_name);


    /** write the class. This type mapping would have been populated right now
     * Note - We always write classes for named complex types
     */
    w2c_schema_compiler_write_complex_type( complier_impl, env, complex_type,
                                               meta_info );
    return AXIS2_SUCCESS;
}

axis2_status_t
w2c_schema_compiler_write_complex_type(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    xml_schema_complex_type_t *complex_type,
                                    w2c_schema_writer_meta_info_t *meta_info )
{
    axis2_hash_t *base_hash = NULL;
    xml_schema_type_t *schema_type = NULL;
    axis2_qname_t *qname = NULL;
    axis2_char_t *qname_str = NULL;

    base_hash = XML_SCHEMA_COMPLEX_TYPE_SUPER_OBJS( complex_type, env);
    schema_type = axis2_hash_get( base_hash, "XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING);
    qname = XML_SCHEMA_TYPE_GET_QNAME( schema_type, env);
    qname_str = AXIS2_QNAME_TO_STRING( qname, env);

    W2C_SCHEMA_WRITER_WRITE( compiler_impl-> writer, env, complex_type,
                             compiler_impl-> processed_type_map, meta_info);
    axis2_hash_set( compiler_impl-> processed_type_meta_info_map, qname_str, 
                                              AXIS2_HASH_KEY_STRING, meta_info);
    return AXIS2_SUCCESS;
}

w2c_schema_writer_meta_info_t*
w2c_schema_compiler_write_process_complex_type( 
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    xml_schema_complex_type_t *complex_type,
                                    xml_scheam_t *parent_schema)
{
    xml_schema_particle_t *particle = NULL;
    w2c_schema_writer_meta_info_t *meta_info = NULL;
    xml_schema_obj_collection_t* attribs = NULL;
    int i = 0;
    int count = 0;
    xml_schema_obj_t *schema_obj = NULL;
    xml_schema_attribute_t *attri = NULL;
    xml_schema_any_attribute_t *any_attri = NULL;
    void *content_model = NULL;

    particle = XML_SCHEMA_COMPLEX_TYPE_GET_PARTICLE( complex_type, env);
    meta_info = w2c_schema_writer_meta_info_create( env);
    if ( particle != NULL )
    {
        w2_schema_compiler_process_particle( particle, meta_info, parent_schema);
    }

    /* process attributes - first look for the explicit attributes*/
    attribs = XML_SCHEMA_COMPLEX_TYPE_GET_ATTRIBUTES( complex_type, env);
    count = XML_SCHEMA_OBJ_COLLECTION_GET_COUNT( attribs, env);
    for ( i = 0; i < count; i ++)
    {
        schema_obj = XML_SCHEMA_OBJ_COLLECTION_GET_ITEM( attribs, env, i );
        if( XML_SCHEMA_ATTRIBUTE == XML_SCHEMA_OBJ_GET_TYPE( schema_obj, env) )
        {
            attri = (xml_schema_attribute_t*)schema_obj;
            w2c_schema_compiler_process_attribute( compiler_impl, env, attri, meta_info);
        }
    }
   
    /* process any attribute */
    /* java note:somehow the xml schema parser does not seem to pickup the any attribute! */ 
    any_attri = XML_SCHEMA_COMPLEX_TYPE_GET_ANY_ATTRIBUTE(complex_type, env);
    if ( any_attri != NULL)
    {
        w2c_schema_compiler_process_any_attribute( compiler_impl, env, any_attri, meta_info);
    }

    /* process content ,either  complex or simple */
    content_model = XML_SCHEMA_COMPLEX_TYPE_GET_CONTENT_MODEL( complex_type, env);
    w2c_schema_compiler_process_content_model( compiler_impl, env, content_model, meta_info, parent_schema);

    return meta_info;
}

axis2_status_t
w2c_schema_compiler_process_content_model(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    void *content_model,
                                    w2c_schema_writer_meta_info_t *meta_info, 
                                    xml_scheam_t *parent_schema)
{
    xml_schema_simple_content_t *simple_content = NULL;
    xml_schema_complex_content_t *complex_content = NULL;

    /** CHECK. is this working?, content_model is a void pointer */
    if( XML_SCHEMA_SIMPLE_CONTENT ==  XML_SCHEMA_OBJ_GET_TYPE( content_model, env))
    {
        simple_content = (xml_schema_simple_content_t*)content_model;
        w2c_schema_compiler_process_simple_content( compiler_impl, env,
                                 simpel_content, meta_info, parent_schema );
    }
    else if( XML_SCHEMA_COMPLEX_CONTENT ==  XML_SCHEMA_OBJ_GET_TYPE( content_model, env))
    {
        complex_content = (xml_schema_complex_content_t*)content_model;
        w2c_schema_compiler_process_complex_content( compiler_impl, env,
                                 complex_content, meta_info, parent_schema );
    }
    return AXIS2_SUCCESS;
}

axis2_status_t
w2c_schema_compiler_process_complex_content(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    xml_schema_complex_content_t *complex_content,
                                    w2c_schema_writer_meta_info_t *meta_info, 
                                    xml_scheam_t *parent_schema)
{
    xml_schema_complex_content_extension_t *extension = NULL;
    xml_schema_complex_content_restriction_t *restriction = NULL;
    axis2_qname_t *base_type_qname = NULL;
    xml_schema_type_t *type = NULL;
    axis2_hash_t *base_hash = NULL;
    xml_schema_complex_type_t *complex_type = NULL;
    xml_schema_simple_type_t *simple_type = NULL;
    axis2_char_t *name = NULL;
    xml_schema_particle_t *particle = NULL;
    axis2_char_t *class_name = NULL;
    void *content = NULL;

    content = W2C_SCHEMA_COMPLEX_CONTENT_GET_CONTENT( complex_content, env);

    if ( XML_SCHEMA_COMPLEX_CONTENT_EXTENSION == 
             XML_SCHEMA_OBJ_GET_TYPE(content, env) )
    {
        extenion = ( xml_schema_complex_content_extension_t*)content;
        base_type_qname = XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_GET_BASE_TYPE_NAME( extension, env);
        if( !w2c_schema_compiler_is_already_processed( compiler_impl, env, base_type_qname) )
        { 
            type = XML_SCHEMA_GET_TYPE_BY_QNAME( parent_schema, env, base_type_qname);
            if (NULL != type)
            {
                if ( XML_SCHEMA_COMPLEX_TYPE = XML_SCHEMA_TYPE_GET_TYPE( type, env ) )
                {
                    base_hash = XML_SCHEMA_COMPLEX_TYPE_SUPER_OBJS( type, env);
                    complex_type = axis2_hash_get( base_hash, "XML_SCHEMA_COMPLEX_TYPE", AXIS2_HASH_KEY_STRING);
                    name = XML_SCHEMA_TYPE_GET_NAME( type, env);
                    if ( NULL != name )
                    {
                        w2c_schema_compiler_processed_named_complex_schema_type( compiler_impl,
                                    env, complex_type, parent_schema );
                    }
                    else
                    {
                        /** extension should always have a name */
                        w2c_messages_print_n_log_error( env, "schema.anonymousExtension");
                        return AXIS2_FAILURE;
                    }
                }
                else if( XML_SCHEMA_SIMPLE_TYPE = XML_SCHEMA_TYPE_GET_TYPE( type, env ) )
                {
                    /* process simple type */
                    base_hash = XML_SCHEMA_COMPLEX_TYPE_SUPER_OBJS( type, env);
                    simple_type = axis2_hash_get( base_hash, "XML_SCHEMA_SIMPLE_TYPE", AXIS2_HASH_KEY_STRING);
                    w2c_schema_compiler_processed_named_simple_schema_type( compiler_impl,
                                    env, simple_type, NULL);
                }
            }
        }
        w2c_schema_compiler_copy_meta_info( compiler_impl, env, meta_info, base_type_qname, parent_schema);

        /* process the particle of this node */
        particle = XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_GET_PARTICLE( extension, env);
        w2c_schema_compiler_process_particle( compiler_impl, env, meta_info, particle, parent_schema);
        /* the particle has been processed, However since this is an extension we need to
         * add the basetype as an extension to the complex type class.
         * The basetype has been processed already
         */
        W2C_SCHEMA_WRITER_META_INFO_SET_EXTENSION( meta_info, env, AXIS2_TRUE);
        /* Note  - this is no array! so the array boolean is always false */
        class_name = w2c_schema_compiler_find_classname( compiler_impl, env, base_type_qname, AXIS2_FALSE);
        W2C_SCHEMA_WRITER_META_INFO_SET_EXTENSION_CLASSNAME( meta_info, env, class_name );

    }
    else if ( XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION == 
             XML_SCHEMA_OBJ_GET_TYPE(content, env) )
    {
        restriction = ( xml_schema_complex_content_restriction_t*)content;
        base_type_qname = XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_GET_BASE_TYPE_NAME( extension, env);
        if( !w2c_schema_compiler_is_already_processed( compiler_impl, env, base_type_qname) )
        { 
            type = XML_SCHEMA_GET_TYPE_BY_QNAME( parent_schema, env, base_type_qname);
            if (NULL != type)
            {
                if ( XML_SCHEMA_COMPLEX_TYPE = XML_SCHEMA_TYPE_GET_TYPE( type, env ) )
                {
                    base_hash = XML_SCHEMA_COMPLEX_TYPE_SUPER_OBJS( type, env);
                    complex_type = axis2_hash_get( base_hash, "XML_SCHEMA_COMPLEX_TYPE", AXIS2_HASH_KEY_STRING);
                    name = XML_SCHEMA_TYPE_GET_NAME( type, env);
                    if ( NULL != name )
                    {
                        w2c_schema_compiler_processed_named_complex_schema_type( compiler_impl,
                                    env, complex_type, parent_schema );
                    }
                    else
                    {
                        /** extension should always have a name */
                        w2c_messages_print_n_log_error( env, "schema.anonymousExtension");
                        return AXIS2_FAILURE;
                    }
                }
                else if( XML_SCHEMA_SIMPLE_TYPE = XML_SCHEMA_TYPE_GET_TYPE( type, env ) )
                {
                    /* this also not possible restriction for simple type */
                    w2c_messages_print_n_log_error( env, "schema.simpletypeRestriction");
                    return AXIS2_FAILURE;
                }
            }
        }

        w2c_schema_compiler_copy_meta_info( compiler_impl, env, meta_info, base_type_qname, parent_schema);

        /* process the particle of this node */
        particle = XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_GET_PARTICLE( extension, env);
        w2c_schema_compiler_process_particle( compiler_impl, env, meta_info, particle, parent_schema);
        
        W2C_SCHEMA_WRITER_META_INFO_SET_RESTRICTION( meta_info, env, AXIS2_TRUE);
        /* Note  - this is no array! so the array boolean is always false */
        class_name = w2c_schema_compiler_find_classname( compiler_impl, env, base_type_qname, AXIS2_FALSE);
        W2C_SCHEMA_WRITER_META_INFO_SET_RESTRICTION_CLASSNAME( meta_info, env, class_name );
    }
    return AXIS2_SUCCESS;
}


axis2_status_t
w2c_schema_compiler_copy_meta_info(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    w2c_schema_writer_meta_info_t *meta_info_child,
                                    axis2_qname_t *base_type_qname,
                                    xml_schema_t *parent_schema )
{
    xml_schema_type_t *type =  NULL;
    axis2_char_t *qname_str = NULL;
    axis2_hash_t *base_hash = NULL;
    xml_schema_complex_type_t *complex_type = NULL;
    void *content_model = NULL;
    xml_schema_simple_content_t *simple_content = NULL;
    xml_schema_complex_content_t *complex_content = NULL;
    xml_schema_complex_content_extension_t *extension = NULL;
    xml_schema_complex_content_extension_t *restriction = NULL;
    axis2_qname_t *qname = NULL;
    void *content = NULL;

    qname_str = AXIS2_QNAME_TO_STRING( base_type_qname, env);
    type = XML_SCHEMA_GET_TYPE_BY_QNAME( parent_schema, env, base_type_qname);
    meta_info = axis2_hash_get( compiler-> processed_type_meta_info_map, qname_str, 
                               AXIS2_HASH_KEY_STRING);

    if ( meta_info != NULL )
    {
        if ( XML_SCHEMA_COMPLEX_TYPE = XML_SCHEMA_TYPE_GET_TYPE( type, env ) )
        {
            base_hash = XML_SCHEMA_COMPLEX_TYPE_SUPER_OBJS( type, env);
            complex_type = axis2_hash_get( base_hash, "XML_SCHEMA_COMPLEX_TYPE", AXIS2_HASH_KEY_STRING);
            content_model = XML_SCHEMA_COMPLEX_TYPE_GET_CONTENT_MODEL( complex_type, env);
           
            /** assuming content_mode is an xml_schema_obj */ 
            if( XML_SCHEMA_COMPLEX_CONTENT ==  XML_SCHEMA_OBJ_GET_TYPE( content_model, env))
            {
                complex_content = (xml_schema_complex_content_t*)content_model;
                content = XML_SCHEMA_COMPLEX_CONTENT_GET_CONTENT( complex_content, env);

                if ( XML_SCHEMA_COMPLEX_CONTENT_EXTENSION == 
                         XML_SCHEMA_OBJ_GET_TYPE(content, env) )
                {
                    extension = (xml_schema_complex_content_extension_t*)content;
                    qname = XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_GET_BASE_TYPE_NAME( extension, env);
                    /*recursively call the copyMetaInfoHierarchy method */
                    w2c_schema_compiler_copy_meta_info( compiler_impl, env, meta_info, qname, parent_schema);
                }
                else if ( XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION == 
                         XML_SCHEMA_COMPLEX_CONTENT_GET_TYPE(complex_content, env) )
                {
                    restriction = (xml_schema_complex_content_restriction_t*)content;
                    qname = XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_GET_BASE_TYPE_NAME( extension, env);
                    /*recursively call the copyMetaInfoHierarchy method */
                    w2c_schema_compiler_copy_meta_info( compiler_impl, env, meta_info, qname, parent_schema);
                }
                else
                {
                    w2c_messages_print_n_log_error( env, "schema.unknownContentError");
                    return AXIS2_FAILURE;
                }
            }
            else if( XML_SCHEMA_SIMPLE_CONTENT ==  XML_SCHEMA_OBJ_GET_TYPE( content_model, env))
            {
                w2c_messages_print_n_log_error( env, "schema.unsupportedContentError");
                return AXIS2_FAILURE;
            }
            else
            {
                w2c_messages_print_n_log_error( env, "schema.unknowContentModelError");
                return AXIS2_FAILURE;
            }
        }
        W2C_SCHEMA_WRITER_META_INFO_SET_PARENT( meta_info_child, env, meta_info);
    }
    return AXIS2_SUCCESS;
}


axis2_status_t
w2c_schema_compiler_process_simple_content(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    w2c_schema_simple_content_t *simple_content,
                                    w2c_schema_writer_meta_info_t *meta_info,
                                    xml_schema_t *parent_schema )
{
    xml_schema_simple_content_extension_t *extension = NULL;
    xml_schema_simple_content_restriction_t *restriction = NULL;
    axis2_qname_t *base_type_qname = NULL;
    xml_schema_type_t *type = NULL;
    axis2_hash_t *base_hash = NULL;
    xml_schema_complex_type_t *complex_type = NULL;
    xml_schema_simple_type_t *simple_type = NULL;
    axis2_char_t *name = NULL;
    void *content = NULL;

    xml_schema_obj_collection_t *attribs = NULL;
    int i = 0;
    int count = 0;
    xml_schema_obj_t *schema_obj = NULL;
    xml_schema_attribute_t *attri = NULL;
    xml_schema_any_attribute_t *any_attri = NULL;

    xml_schema_obj_collection_t *facets = NULL;


    content = W2C_SCHEMA_SIMPLE_CONTENT_GET_CONTENT( complex_content, env);

    if ( XML_SCHEMA_SIMPLE_CONTENT_EXTENSION == 
             XML_SCHEMA_OBJ_GET_TYPE(content, env) )
    {
        extenion = ( xml_schema_simple_content_extension_t*)content;
        base_type_qname = XML_SCHEMA_SIMPLE_CONTENT_EXTENSION_GET_BASE_TYPE_NAME( extension, env);
        if( !w2c_schema_compiler_is_already_processed( compiler_impl, env, base_type_qname) )
        { 
            type = XML_SCHEMA_GET_TYPE_BY_QNAME( parent_schema, env, base_type_qname);
            if (NULL != type)
            {
                if ( XML_SCHEMA_COMPLEX_TYPE = XML_SCHEMA_TYPE_GET_TYPE( type, env ) )
                {
                    base_hash = XML_SCHEMA_COMPLEX_TYPE_SUPER_OBJS( type, env);
                    complex_type = axis2_hash_get( base_hash, "XML_SCHEMA_COMPLEX_TYPE", AXIS2_HASH_KEY_STRING);
                    name = XML_SCHEMA_TYPE_GET_NAME( type, env);
                    if ( NULL != name )
                    {
                        w2c_schema_compiler_processed_named_complex_schema_type( compiler_impl,
                                    env, complex_type, parent_schema );
                    }
                    else
                    {
                        /** extension should always have a name */
                        w2c_messages_print_n_log_error( env, "schema.anonymousExtension");
                        return AXIS2_FAILURE;
                    }
                }
                else if( XML_SCHEMA_SIMPLE_TYPE = XML_SCHEMA_TYPE_GET_TYPE( type, env ) )
                {
                    /* process simple type */
                    base_hash = XML_SCHEMA_COMPLEX_TYPE_SUPER_OBJS( type, env);
                    simple_type = axis2_hash_get( base_hash, "XML_SCHEMA_SIMPLE_TYPE", AXIS2_HASH_KEY_STRING);
                    w2c_schema_compiler_processed_named_simple_schema_type( compiler_impl,
                                    env, simple_type, NULL);
                }
            }
        }
        w2c_schema_compiler_process_simple_extension_base_type( compiler_impl, env, base_type_qname, meta_info);
    
        /* process attributes - first look for the explicit attributes*/
        attribs = XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_GET_ATTRIBUTES( extension, env);
        count = XML_SCHEMA_OBJ_COLLECTION_GET_COUNT( attribs, env);
        for ( i = 0; i < count; i ++)
        {
            schema_obj = XML_SCHEMA_OBJ_COLLECTION_GET_ITEM( attribs, env, i );
            if( XML_SCHEMA_ATTRIBUTE == XML_SCHEMA_OBJ_GET_TYPE( schema_obj, env) )
            {
                attri = (xml_schema_attribute_t*)schema_obj;
                w2c_schema_compiler_process_attribute( compiler_impl, env, attri, meta_info);
            }
        }
    
        /* process any attribute */
        any_attri = XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_GET_ANY_ATTRIBUTE( extension, env);
        if ( any_attri != NULL)
        {
            w2c_schema_compiler_process_any_attribute( compiler_impl, env, any_attri, meta_info);
        }
   
    }
    else if ( XML_SCHEMA_SIMPLE_CONTENT_RESTRICTION == 
             XML_SCHEMA_OBJ_GET_TYPE(content, env) )
    {
        restriction = ( xml_schema_simple_content_restriction_t*)content;
        base_type_qname = XML_SCHEMA_SIMPLE_CONTENT_RESTRICTION_GET_BASE_TYPE_NAME( extension, env);
        if( !w2c_schema_compiler_is_already_processed( compiler_impl, env, base_type_qname) )
        { 
            type = XML_SCHEMA_GET_TYPE_BY_QNAME( parent_schema, env, base_type_qname);
            if (NULL != type)
            {
                if ( XML_SCHEMA_COMPLEX_TYPE = XML_SCHEMA_TYPE_GET_TYPE( type, env ) )
                {
                    base_hash = XML_SCHEMA_COMPLEX_TYPE_SUPER_OBJS( type, env);
                    complex_type = axis2_hash_get( base_hash, "XML_SCHEMA_COMPLEX_TYPE", AXIS2_HASH_KEY_STRING);
                    name = XML_SCHEMA_TYPE_GET_NAME( type, env);
                    if ( NULL != name )
                    {
                        w2c_schema_compiler_processed_named_complex_schema_type( compiler_impl,
                                    env, complex_type, parent_schema );
                    }
                    else
                    {
                        /** extension should always have a name */
                        w2c_messages_print_n_log_error( env, "schema.anonymousExtension");
                        return AXIS2_FAILURE;
                    }
                }
                else if( XML_SCHEMA_SIMPLE_TYPE = XML_SCHEMA_TYPE_GET_TYPE( type, env ) )
                {
                    /* process simple type */
                    base_hash = XML_SCHEMA_COMPLEX_TYPE_SUPER_OBJS( type, env);
                    simple_type = axis2_hash_get( base_hash, "XML_SCHEMA_SIMPLE_TYPE", AXIS2_HASH_KEY_STRING);
                    w2c_schema_compiler_processed_named_simple_schema_type( compiler_impl,
                                    env, simple_type, NULL);
                }
            }
        }

        w2c_schema_compiler_process_simple_restriction_base_type( compiler_impl, env, base_type_qname, meta_info);
        
        facets = XML_SCHEMA_SIMPLE_CONTENT_RESTRICTION_GET_FACETS( restriction, env);
        w2_schema_compiler_process_facets( compiler_impl, env, facets, meta_info);

    }
    return AXIS2_SUCCESS;
}

axis2_status_t
w2c_schema_compiler_process_simple_extension_base_type(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    axis2_qname_t *ext_base_type,
                                    w2c_schema_writer_meta_info_t *meta_info)
{
    axis2_char_t *class_name = NULL;
    axis2_qname_t *changed_qname = NULL;
    axis2_char_t *qname_str = NULL;
    
    qname_str = AXIS2_QNAME_TO_STRING( ext_base_type, env);

    class_name = w2c_schema_compiler_find_class_name( compiler_impl, env,
                                     ext_base_type, AXIS2_FALSE);
    changed_qname = axis2_hash_get( compiler_impl-> changed_type_map, qname_str, 
                               AXIS2_HASH_KEY_STRING);

    if ( changed_qname == NULL )
    {
       changed_qname = qname;
    }
    W2C_SCHEMA_WRITER_META_INFO_REGISTER_MAP(meta_info, env, 
                               qname, changed_qname, class_name,
                               W2C_SCHEMA_CONSTS_ELEMENT_TYPE);

    return AXIS2_SUCCESS;
}

axis2_status_t
w2c_schema_compiler_process_simple_restriction_base_type(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    axis2_qname_t *res_base_type,
                                    w2c_schema_writer_meta_info_t *meta_info)
{
    axis2_char_t *class_name = NULL;
    axis2_qname_t *changed_qname = NULL;
    axis2_char_t *qname_str = NULL;
    
    qname_str = AXIS2_QNAME_TO_STRING( ext_base_type, env);

    class_name = w2c_schema_compiler_find_class_name( compiler_impl, env,
                                     res_base_type, AXIS2_FALSE);
    changed_qname = axis2_hash_get( compiler_impl-> changed_type_map, qname_str, 
                               AXIS2_HASH_KEY_STRING);

    if ( changed_qname == NULL )
    {
       changed_qname = qname;
    }
    W2C_SCHEMA_WRITER_META_INFO_REGISTER_MAP(meta_info, env, 
                               qname, changed_qname, class_name,
                               W2C_SCHEMA_CONSTS_ELEMENT_TYPE);
    W2C_SCHEMA_WRITER_META_INFO_SET_RESTRICTIO_BASE_TYPE( meta_info, env, res_base_type);

    return AXIS2_SUCCESS;
}


axis2_status_t
w2c_schema_compiler_process_facets(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    xml_schema_obj_collection_t *facets,
                                    w2c_schema_writer_meta_info_t *meta_info)
{
    xml_schema_obj_t *schema_obj = NULL;
    axis2_bool_t *enum_start = AXIS2_FALSE;
    int i = 0;
    int count = 0;
    xml_schema_facets_t *facets = NULL;
    axis2_char_t *facets_value = NULL;
    axis2_bool_t enum_start = AXIS2_FALSE;

    count = XML_SCHEMA_OBJ_COLLECTION_GET_COUNT( facets, env);
    for ( i = 0; i < count; i ++)
    {
        schema_obj = XML_SCHEMA_OBJ_COLLECTION_GET_ITEM( attribs, env, i );
        if( XML_SCHEMA_PATTERN_FACETS == XML_SCHEMA_OBJ_GET_TYPE( schema_obj, env) )
        {
            base_hash = XML_SCHEMA_OBJ_SUPER_OBJS( schema_obj, env);
            facets = (xml_schema_facets_t*)axis2_hash_get( base_hash, "XML_SCHEMA_FACETS", AXIS2_HASH_KEY_STRING);
            facets_value = XML_SCHEMA_FACETS_GET_VALUE( facets, env);
            W2C_SCHEMA_WRITER_META_INFO_SET_PATTERN_FACETS( meta_info, env, facets_value);
        }
        else if( XML_SCHEMA_ENUMARATION_FACET == XML_SCHEMA_OBJ_GET_TYPE( schema_obj, env) )
        {
            base_hash = XML_SCHEMA_OBJ_SUPER_OBJS( schema_obj, env);
            facets = (xml_schema_facets_t*)axis2_hash_get( base_hash, "XML_SCHEMA_FACET", AXIS2_HASH_KEY_STRING);
            facets_value = XML_SCHEMA_FACETS_GET_VALUE( facets, env);
            if ( !enum_start )
            {
                W2C_SCHEMA_WRITER_META_INFO_SET_ENUM_FACETS( meta_info, env, facets_value);
                enum_start = AXIS2_TRUE;
            }
            else
            {
                W2C_SCHEMA_WRITER_META_INFO_ADD_ENUM_FACETS( meta_info, env, facets_value);
            }
        }
        else if( XML_SCHEMA_LENGTH_FACET == XML_SCHEMA_OBJ_GET_TYPE( schema_obj, env) )
        {
            base_hash = XML_SCHEMA_OBJ_SUPER_OBJS( schema_obj, env);
            facets = (xml_schema_facets_t*)axis2_hash_get( base_hash, "XML_SCHEMA_FACET", AXIS2_HASH_KEY_STRING);
            facets_value = XML_SCHEMA_FACETS_GET_VALUE( facets, env);
            W2C_SCHEMA_WRITER_META_INFO_SET_LENGTH_FACETS( meta_info, env, facets_value);
        }
        else if( XML_SCHEMA_MAX_EXCLUSIVE_FACET== XML_SCHEMA_OBJ_GET_TYPE( schema_obj, env) )
        {
            base_hash = XML_SCHEMA_OBJ_SUPER_OBJS( schema_obj, env);
            facets = (xml_schema_facets_t*)axis2_hash_get( base_hash, "XML_SCHEMA_FACET", AXIS2_HASH_KEY_STRING);
            facets_value = XML_SCHEMA_FACETS_GET_VALUE( facets, env);
            W2C_SCHEMA_WRITER_META_INFO_SET_MAX_EXCLUSIVE_FACETS( meta_info, env, facets_value);
        }
        else if( XML_SCHEMA_MIN_EXCLUSIVE_FACET== XML_SCHEMA_OBJ_GET_TYPE( schema_obj, env) )
        {
            base_hash = XML_SCHEMA_OBJ_SUPER_OBJS( schema_obj, env);
            facets = (xml_schema_facets_t*)axis2_hash_get( base_hash, "XML_SCHEMA_FACET", AXIS2_HASH_KEY_STRING);
            facets_value = XML_SCHEMA_FACETS_GET_VALUE( facets, env);
            W2C_SCHEMA_WRITER_META_INFO_SET_MIN_EXCLUSIVE_FACETS( meta_info, env, facets_value);
        }
        else if( XML_SCHEMA_MAX_INCLUSIVE_FACET== XML_SCHEMA_OBJ_GET_TYPE( schema_obj, env) )
        {
            base_hash = XML_SCHEMA_OBJ_SUPER_OBJS( schema_obj, env);
            facets = (xml_schema_facets_t*)axis2_hash_get( base_hash, "XML_SCHEMA_FACET", AXIS2_HASH_KEY_STRING);
            facets_value = XML_SCHEMA_FACETS_GET_VALUE( facets, env);
            W2C_SCHEMA_WRITER_META_INFO_SET_MAX_INCLUSIVE_FACETS( meta_info, env, facets_value);
        }
        else if( XML_SCHEMA_MIN_INCLUSIVE_FACET== XML_SCHEMA_OBJ_GET_TYPE( schema_obj, env) )
        {
            base_hash = XML_SCHEMA_OBJ_SUPER_OBJS( schema_obj, env);
            facets = (xml_schema_facets_t*)axis2_hash_get( base_hash, "XML_SCHEMA_FACET", AXIS2_HASH_KEY_STRING);
            facets_value = XML_SCHEMA_FACETS_GET_VALUE( facets, env);
            W2C_SCHEMA_WRITER_META_INFO_SET_MIN_INCLUSIVE_FACETS( meta_info, env, facets_value);
        }
        else if( XML_SCHEMA_MAX_LENGTH_FACET== XML_SCHEMA_OBJ_GET_TYPE( schema_obj, env) )
        {
            base_hash = XML_SCHEMA_OBJ_SUPER_OBJS( schema_obj, env);
            facets = (xml_schema_facets_t*)axis2_hash_get( base_hash, "XML_SCHEMA_FACET", AXIS2_HASH_KEY_STRING);
            facets_value = XML_SCHEMA_FACETS_GET_VALUE( facets, env);
            W2C_SCHEMA_WRITER_META_INFO_SET_MAX_LENGTH_FACETS( meta_info, env, facets_value);
        }
        else if( XML_SCHEMA_MIN_LENGTH_FACET== XML_SCHEMA_OBJ_GET_TYPE( schema_obj, env) )
        {
            base_hash = XML_SCHEMA_OBJ_SUPER_OBJS( schema_obj, env);
            facets = (xml_schema_facets_t*)axis2_hash_get( base_hash, "XML_SCHEMA_FACET", AXIS2_HASH_KEY_STRING);
            facets_value = XML_SCHEMA_FACETS_GET_VALUE( facets, env);
            W2C_SCHEMA_WRITER_META_INFO_SET_MIN_LENGTH_FACETS( meta_info, env, facets_value);
        }
    }
    return AXIS2_SUCCESS;
}

axis2_status_t
w2c_schema_compiler_process_any_attributes(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    w2c_schema_writer_meta_info_t *meta_info,
                                    xml_schema_any_attribute_t *any_attri )
{
    axis2_qname_t *qname = NULL;

    qname = axis2_qname_create( env, 
            W2C_SCHEMA_COMPILER_EXTRA_ATTRIBUTE_FIELD_NAME, NULL, NULL );

    W2C_SCHEMA_WRITER_META_INFO_REGISTER_MAP(meta_inf, env, 
                               qname, NULL , "DEFAULT_ARRAY",
                               W2C_SCHEMA_CONSTS_ANY_TYPE);
    W2C_SCHEMA_WRITER_META_INFO_ADD_STATUS( meta_info, qname, W2C_SCHEMA_CONSTS_ATTRIBUTE_TYPE);
    W2C_SCHEMA_WRITER_META_INFO_ADD_STATUS( meta_info, qname, W2C_SCHEMA_CONSTS_ARRAY_TYPE);
}

axis2_status_t
w2c_schema_compiler_process_attribute(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    w2c_schema_writer_meta_info_t *meta_info,
                                    xml_schema_attribute_t *attri )
{
    axis2_qname_t *schema_type_name = NULL;
    axis2_qname_t *mapped_qname = NULL;
    axis2_char_t *qname_str = NULL;
    axis2_qname_t *attr_qname = NULL;

    schema_type_name = XML_SCHEMA_ATTRIBUTE_GET_SCHEMA_TYPE_NAME( attri, env);
    qname_str = AXIS2_QNAME_TO_STRING( schema_type_name, env);
    mapped_qname = axis2_hash_get( compiler_impl-> base_schema_type_map, qname_str,
                                      AXIS2_HASH_KEY_STRING);
    /* for now we assume (!!!) that attributes refer to standard types only */
    if ( mapped_qname != NULL )
    {
        attr_qname = XML_SCHEMA_ATTRIBUTE_GET_QNAME( attri, env);
        qname_str = AXIS2_QNAME_TO_STRING( mapped_qname, env);
        W2C_SCHEMA_WRITER_META_INFO_REGISTER_MAP(meta_inf, env, 
                               attr_qname, schema_type_name,  qname_str,
                               W2C_SCHEMA_CONSTS_ATTRIBUTE_TYPE);
    }
    else
    {
        /*TODO: handle for custom types */
    }
}

axis2_status_t
w2c_schema_compiler_process_particle(
                                    w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    xml_schema_particle_t *particle,
                                    w2c_schema_writer_meta_info_t *meta_info,
                                    xml_schema_t *parent_schema )
{
    axis2_hash_t *base_hash = NULL;
    xml_schema_sequence_t *sequence = NULL;
    xml_schema_all_t *all = NULL;
    xml_schema_choise_t *choise = NULL;
    xml_schema_obj_collection_t *items = NULL;

    if( XML_SCHEMA_SEQUENCE == XML_SCHEMA_PARTICLE_GET_TYPE( particle, env) )
    {
        sequence = (xml_schema_sequence_t*)particle;
        items = XML_SCHEMA_GROUP_BASE_GET_ITEMS( sequence, env);
        w2c_schema_compiler_process( compiler_impl, env, items, meta_info, 
                           AXIS2_TRUE, parent_schema );  

    }
    else if( XML_SCHEMA_ALL == XML_SCHEMA_PARTICLE_GET_TYPE( particle, env) )
    {
        all = (xml_schema_all_t*)particle;
        items = XML_SCHEMA_GROUP_BASE_GET_ITEMS( all, env);
        w2c_schema_compiler_process( compiler_impl, env, items, meta_info, 
                           AXIS2_FALSE, parent_schema );  
    }
    else if( XML_SCHEMA_CHOISE == XML_SCHEMA_PARTICLE_GET_TYPE( particle, env) )
    {
        choise = (xml_schema_choise_t*)particle;
        items = XML_SCHEMA_GROUP_BASE_GET_ITEMS( choise, env);
        w2c_schema_compiler_process( compiler_impl, env, items, meta_info, 
                           AXIS2_FALSE, parent_schema );  
        W2C_SCHEMA_WRITER_META_INFO_SET_CHOISE(meta_inf, env, AXIS2_TRUE);
    }
    return AXIS2_SUCCESS;
}

axis2_status_t
w2c_schema_compiler_process( w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    xml_schema_obj_collection_t *items,
                                    w2c_schema_writer_meta_info_t *meta_info,
                                    axis2_bool_t order,
                                    xml_schema_t *parent_schema )
{
    axis2_hash_t *processed_element_array_status_map = NULL;
    axis2_hash_t *processed_element_type_map = NULL;
    axis2_hash_t *element_order_map = NULL;
    axis2_hash_t *local_nillable_list = NULL;

    xml_schema_obj_t *obj = NULL;
    int i = 0;
    int count = 0;
    xml_schema_element_t *ele = NULL;
    axis2_bool_t is_array = AXIS2_FALSE;
    axis2_qname_t *qname = NULL;
    axis2_char_t *qname_str = NULL;
    xml_schema_any_t *any = NULL;
    void *key = NULL;
    void *value = NULL;
    axis2_hash_index_t *hi = NULL;
    int starting_item_number_order = 0;
    axis2_char_t *class_name = NULL;

    xml_schema_type_t *schema_type = NULL;
    axis2_qname_t *schema_type_qname = NULL;
    axis2_qname_t *referenced_qname = NULL;
    xml_schema_element_t *element_by_qname = NULL;

    xml_schema_particle_t *particle = NULL;
    int index = 0;
    axis2_qname_t *any_element_field_qname = NULL;

    processed_element_array_status_map = axis2_hash_make( env);
    element_order_map = axis2_hash_make( env);
    processed_element_type_map = axis2_hash_make( env);
    local_nillable_list = axis2_hash_make( env);

    count = XML_SCHEMA_OBJ_COLLECTION_GET_COUNT( items, env);
    for ( i = 0; i < count; i ++)
    {
        schema_obj = XML_SCHEMA_OBJ_COLLECTION_GET_ITEM( items , env, i );
        if( XML_SCHEMA_ELEMENT == XML_SCHEMA_OBJ_GET_TYPE( schema_obj, env) )
        {
            base_hash = XML_SCHEMA_OBJ_SUPER_OBJS( schema_obj, env);
            ele = (xml_schema_element_t*)axis2_hash_get( base_hash, "XML_SCHEMA_ELEMENT", AXIS2_HASH_KEY_STRING);
            is_array = W2C_SCHEMA_COMPILER_IS_ARRAY( compiler_impl, env, ele );
            w2c_schema_compiler_process_element( compiler_impl, env,
                                     processed_element_type_map,local_nillable_list, parent_schema);
            qname = XML_SCHEMA_ELEMENT_GET_QNAME( ele, env);
            qname_str = AXIS2_QNAME_TO_STRING( qname, env);
            /** CHECK: putting a pointer to hash key*/
            axis2_hash_set( processed_element_array_status_map, ele , sizeof(void*), (void*)is_array);
            if ( order)
            {
                axis2_hash_set( element_order_map, qname_str, AXIS2_HASH_KEY_STRING, i );
            }
        }
        else if( XML_SCHEMA_ANY == XML_SCHEMA_OBJ_GET_TYPE( schema_obj, env) )
        {
            base_hash = XML_SCHEMA_OBJ_SUPER_OBJS( schema_obj, env);
            any = (xml_schema_any_t*)axis2_hash_get( base_hash, "XML_SCHEMA_ANY", AXIS2_HASH_KEY_STRING);
            qname = axis2_qname_create( env, XML_SCHEMA_COMPILER_ANY_ELEMENT_FIELD_NAME,
                                        NULL, NULL );
            qname_str = AXIS2_QNAME_TO_STRING( qname, env);
            /** CHECK: putting a pointer to hash key*/
            axis2_hash_set( processed_element_type_map, any, sizeof(void*), (void*)is_array);
            /* any can also be inside a sequence */
            if ( order)
            {
                axis2_hash_set( element_order_map, qname_str, AXIS2_HASH_KEY_STRING, i );
            }
        }
    }

    /* loop through the processed items and add them to the matainf object */
    starting_item_number_order = W2C_SCHEMA_WRITER_META_INFO_GET_ORDER_START_POINT( meta_info, env);
    for (hi = axis2_hash_first( processed_element_array_status_map, env);
                hi; hi = axis2_hash_next(env, hi))
    {
        axis2_hash_this(hi, (void*)&key, NULL, (void*)&value);
        if( XML_SCHEMA_ELEMENT == XML_SCHEMA_OBJ_GET_TYPE( key, env) )
        {
            ele = (xml_schema_element_t*) key;
            qname = XML_SCHEMA_ELEMENT_GET_QNAME( ele, env);
            if ( qname != NULL )
            {
                qname_str = AXIS2_QNAME_TO_STRING( qname, env);
                class_name = (axis2_char_t*)axis2_hash_get( compiler_impl-> processed_element_type_map,
                                           qname_str, AXIS2_HASH_KEY_STRING);
                /* always get the schema type name from the schema it-self */
                schema_type = XML_SCHEMA_ELEMENT_GET_SCHEMA_TYPE( ele, env);
                if ( schema_type != NULL )
                {
                    schema_type_qname = XML_SCHEMA_TYPE_GET_QNAME( schema_type, env);
                }
                else
                {
                    schema_type_qname = XML_SCHEMA_ELEMENT_GET_SCHEMA_TYPE_QNAME( ele, env);
                }

                referenced_qname = qname;

                W2C_SCHEMA_WRITER_META_INFO_REGISTER_MAP(meta_inf, env, 
                               referenced_qname, schema_type_qname,  class_name,
                               value?W2C_SCHEMA_CONSTS_ARRAY_TYPE:W2C_SCHEMA_CONSTS_ELEMENT_TYPE);
                /**note: value hold is that an array or not */
            }
            else
            {
                /** if it is referenced */
                referenced_qname = XML_SCHEMA_ELEMENT_GET_REF_QNAME( ele, env);
                class_name = w2c_schema_compiler_find_ref_class_name( referenced_qname,
                                                           (axis2_bool_t)value);
                element_by_qname = XML_SCHEMA_GET_ELEMENT_BY_QNAME( parent_schema, env, referenced_qname );
                schema_type_qname = XML_SCHEMA_ELEMENT_GET_SCHEMA_TYPE_QNAME( element_by_qname, env);

                W2C_SCHEMA_WRITER_META_INFO_REGISTER_MAP(meta_inf, env, 
                               referenced_qname, schema_type_qname,  class_name,
                               value?W2C_SCHEMA_CONSTS_ARRAY_TYPE:W2C_SCHEMA_CONSTS_ELEMENT_TYPE);
            }
            /* register the occurence counts */
            base_hash = XML_SCHEMA_ELEMENT_SUPER_OBJS( ele, env);
            particle = axis2_hash_get( base_hash, "XML_SCHEMA_PARTICLE", AXIS2_HASH_KEY_STRING);
            max_occurs = XML_SCHEMA_PARTICLE_GET_MAX_OCCURS( particle, env);
            min_occurs = XML_SCHEMA_PARTICLE_GET_MIN_OCCURS( particle, env);

            W2C_SCHEMA_WRITER_META_INFO_ADD_MAX_OCCURS( referenced_qname, max_occurs );
            W2C_SCHEMA_WRITER_META_INFO_ADD_MIN_OCCURS( referenced_qname, min_occurs );
            

            qname_str = AXIS2_QNAME_TO_STRING( qname, env);
            /* we need the order to be preserved. So record the order also */
            if ( order)
            {
                index = (int)axis2_hash_get( element_order_map, qname_str, AXIS2_HASH_KEY_STRING);
                W2C_SCHEMA_WRITER_META_INFO_REGISTER_QNAME_INDEX( meta_info, env, referenced_qname,
                                                   index+ starting_item_number_order);
            }
            /* get the nillable state and register that on the metainf holder */
            if ( axis2_hash_get( local_nillable_list, qname_str, AXIS2_HASH_KEY_STRING) != NULL)
            {
                W2C_SCHEMA_WRITER_META_INFO_REGISTER_NILLABLE_QNAME( meta_info, env, qname);
            }
            /* get the binary state and add that to the status map */
            if ( w2c_schema_compiler_is_binary( compiler_impl, env, ele ) )
            {
                W2C_SCHEMA_WRITER_META_INFO_ADD_STATUS( meta_info, env, qname,
                        W2C_SCHEMA_CONSTS_BINARY_TYPE);
            }

        } 
        if( XML_SCHEMA_ANY == XML_SCHEMA_OBJ_GET_TYPE( key, env) )
        {
            ele = (xml_schema_any_t*) key;
            any_element_field_qname = axis2_qname_create( env, XML_SCHEMA_COMPILER_ANY_ELEMENT_FIELD_NAME,
                                        NULL, NULL );
            W2C_SCHEMA_WRITER_META_INFO_REGISTER_MAP(meta_inf, env, 
                               any_element_field_qname, NULL,  value?"DEFAULT_CLASS_ARRAY_NAME":"DEFAULT_CLASS_NAME"
                               W2C_SCHEMA_CONSTS_ANY_TYPE);

            /* register the occurence counts */
            base_hash = XML_SCHEMA_ELEMENT_SUPER_OBJS( ele, env);
            particle = axis2_hash_get( base_hash, "XML_SCHEMA_PARTICLE", AXIS2_HASH_KEY_STRING);
            max_occurs = XML_SCHEMA_PARTICLE_GET_MAX_OCCURS( particle, env);
            min_occurs = XML_SCHEMA_PARTICLE_GET_MIN_OCCURS( particle, env);

            W2C_SCHEMA_WRITER_META_INFO_ADD_MAX_OCCURS( referenced_qname, max_occurs );
            W2C_SCHEMA_WRITER_META_INFO_ADD_MIN_OCCURS( referenced_qname, min_occurs );
            

            qname_str = AXIS2_QNAME_TO_STRING( qname, env);
            /* we need the order to be preserved. So record the order also */
            if ( order)
            {
                index = (int)axis2_hash_get( element_order_map, qname_str, AXIS2_HASH_KEY_STRING);
                W2C_SCHEMA_WRITER_META_INFO_REGISTER_QNAME_INDEX( meta_info, env, referenced_qname,
                                                   index+ starting_item_number_order);
            }
            /* get the binary state and add that to the status map */
            if ( w2c_schema_compiler_is_binary( compiler_impl, env, ele ) )
            {
                W2C_SCHEMA_WRITER_META_INFO_ADD_STATUS( meta_info, env, qname,
                        W2C_SCHEMA_CONSTS_BINARY_TYPE);
            }


        }
    }
    
    W2C_SCHEMA_WRITER_META_INFO_SET_ORDERED( meta_info, env, order);

    return AXIS2_SUCCESS;
}

axis2_bool_t
w2c_schema_compiler_is_binary(  w2c_schema_compiler_impl_t *compiler_impl,
                                const axis2_env_t *env,
                                xml_schema_element_t *ele )
{
    xml_schema_type_t *schema_type = NULL;
    axis2_qname_t * qname = NULL;
    axis2_char_t *qname_str = NULL;

    schema_type = XML_SCHEMA_ELEMENT_GET_SCHEMA_TYPE( ele, env);
    if ( schema_type != NULL )
    {
        qname = XML_SCHEMA_TYPE_GET_QNAME( schema_type, env);
        qname_str = AXIS2_QNAME_TO_STRING( qname, env);
        if ( 0 == AXIS2_STRCMP(qname_str, W2C_SCHEMA_CONSTS_XSD_BASE64) )
        {
            return AXIS2_TRUE;
        }
    }
    return AXIS2_FALSE;
}
 

axis2_status_t
w2c_schema_compiler_process( w2c_schema_compiler_impl_t *compiler_impl,
                                    const axis2_env_t *env,
                                    xml_schema_simple_type_t *simple_type,
                                    xml_scheme_element_t *ele )
{
    xml_schema_simple_type_content_t *content = NULL;
    xml_schema_simple_type_restriction_t *restriction = NULL;
    axis2_qname_t *base_type_qname = NULL;
    axis2_char_t *qname_str = NULL;
    axis2_char_t *class_name = NULL;
    axis2_qname_t *type_qname = NULL;
    xml_schema_simple_type_t *rest_base = NULL;
    
    content = XML_SCHEMA_SIMPLE_TYPE_CONTENT_GET_CONTENT( simple_type, env);
    if ( content !=  NULL)
    {
        if ( XML_SCHEMA_SIMPLE_TYPE_RESTRICTION = 
                XML_SCHEMA_SIMPLE_TYPE_CONTENT_GET_TYPE( content , env ) )
        {
            base_hash = XML_SCHEMA_SIMPLE_TYPE_CONTENT_SUPER_OBJS( content, env);
            restriction = axis2_hash_get( base_hash, "XML_SCHEMA_SIMPLE_TYPE_RESTRICTION", AXIS2_HASH_KEY_STRING);

            base_type_qname = XML_SCHEMA_SIMPLE_TYPE_RESTRICTION_GET_BASE_TYPE_NAME( restriction, env);
            qname_str = AXIS2_QNAME_TO_STRING( base_type_qname, env);
            class_name = axis2_hash_get( compiler_impl-> base_schema_type_map, qname_str, AXIS2_HASH_KEY_STRING);

            if ( class_name != NULL )
            {
                type_qname = xml_schema_simple_type_get_qname( simple_type, env);
                qname_str = AXIS2_QNAME_TO_STRING( type_qname, env);
                /* this is a basic xsd datatype. Populate the map and populate
                 * the mappings map
                 */
                axis2_hash_set( compiler_impl-> simple_types_map, qname_str,
                                      AXIS2_HASH_KEY_STRING, class_name);
                /* set the old schema type QName and the new schema type QName */
                axis2_hash_set(  compiler_impl-> changed_type_map, qname_str, 
                                      AXIS2_HASH_KEY_STRING, base_type_qname);

                /* add the class name to the meta info map */
                /** TODO:meta_info ignored */
            }
            else
            {
                /* recurse*/
                rest_base_type = XML_SCHEMA_SIMPLE_TYPE_RESTRICTION_GET_BASE_TYPE( restriction, env);
                w2c_schema_compiler_process( compiler_impl, env, rest_base_type, ele );
            }
        } 
        else if ( XML_SCHEMA_SIMPLE_TYPE_UNION = 
                XML_SCHEMA_SIMPLE_TYPE_CONTENT_GET_TYPE( content , env ) )
        {
            w2c_messages_print_n_log_error( env, "schema.unsupportedContentError");
            return AXIS2_FAILURE;
        }
        else if ( XML_SCHEMA_SIMPLE_TYPE_LIST = 
                XML_SCHEMA_SIMPLE_TYPE_CONTENT_GET_TYPE( content , env ) )
        {
            w2c_messages_print_n_log_error( env, "schema.unsupportedContentError");
            return AXIS2_FAILURE;
        }
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_schema_compiler_compile( w2c_schema_compiler_t *compiler,
                const axis2_env_t *env,
                xml_schema_t *xml_schema)
{
    return w2c_schema_compiler_compile_inner( compiler, env, xml_schema, AXIS2_FALSE);
}
