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

#ifndef AXIS2_OM_DOCUMENT_H
#define AXIS2_OM_DOCUMENT_H

#include <axis2_env.h>
#include <axis2_om_node.h>
#include <axis2_defines.h>

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @file axis2_om_document.h
 * @brief om_document represents an XML document
 */

#define CHAR_SET_ENCODING "UTF-8"
#define XML_VERSION	"1.0"

    struct axis2_om_document;
    struct axis2_om_document_ops;
    struct axis2_om_stax_builder;

/**
 * @defgroup axis2_om_document OM Document
 * @ingroup axis2_om 
 * @{
 */

  /** 
    * @brief OM document operations struct
    * Encapsulator struct for operations of axis2_om_document_t
    */
 AXIS2_DECLARE_DATA   typedef struct axis2_om_document_ops
    {

      /** 
        * Frees document struct
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.        
        * @param document pointer to document struct to be freed
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE.
        */
        axis2_status_t (AXIS2_CALL *free) (struct axis2_om_document *document,
                                           axis2_env_t **env);

      /**
        * Builds the next node if the builder is not finished with input xml stream
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.        
        * @param document document whose next node is to be built. Mandatory, cannot be NULL
        * @return pointer to the next node. NULL on error.
        */
        axis2_om_node_t* (AXIS2_CALL *build_next) (struct axis2_om_document *document,
                                                   axis2_env_t **env);

      /**
        * adds the child node to the document. To the back of the children list.
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.        
        * @param document document to add the child. Mandatory, cannot be NULL.
        * @param child child node to be added. Mandatory, cannot be NULL.
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE.
        */
        axis2_status_t (AXIS2_CALL *add_child)(struct axis2_om_document * document,
                                               axis2_env_t **env,
                                               axis2_om_node_t * child);

      /**
        * Gets the root element of the document.
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.        
        * @param document document to return the root of
        * @return returns a pointer to the root node. If no root present, this method tries to 
        *             build the root. Returns NULL on error. 
        */
        axis2_om_node_t* (AXIS2_CALL *get_root_element)(struct axis2_om_document *document,
                                                        axis2_env_t **env);
                                                        
        axis2_status_t (AXIS2_CALL *set_root_element)(struct axis2_om_document *document,
                                                       axis2_env_t **env,
                                                       axis2_om_node_t *om_node);
        
        axis2_om_node_t* (AXIS2_CALL *build_all)(struct axis2_om_document *document,
                                                axis2_env_t **env);            


    } axis2_om_document_ops_t;

  /**
    * \brief OM document struct
    * Handles the XML document in OM
    */
    typedef struct axis2_om_document
    {
        /** operations of document struct */
        axis2_om_document_ops_t *ops;
      
    } axis2_om_document_t;

  /**
    * creates a document
    * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
    * @param root pointer to document's root node. Optional, can be NULL
    * @param builder pointer to xml builder 
    * @return pointer to the newly created document.
    */
    AXIS2_DECLARE(axis2_om_document_t *)
    axis2_om_document_create (axis2_env_t **env,
                              axis2_om_node_t * root,
                              struct axis2_om_stax_builder *builder);

/** frees given document */
#define AXIS2_OM_DOCUMENT_FREE(document,env) \
        ((document)->ops->free(document,env))
        
/** adds a child to document */
#define AXIS2_OM_DOCUMENT_ADD_CHILD(document,env, child) \
        ((document)->ops->add_child(document,env,child))
        
/** builds next node of document */
#define AXIS2_OM_DOCUMENT_BUILD_NEXT(document,env) \
        ((document)->ops->build_next(document,env))
        
/** gets the root eleemnt of given document */
#define AXIS2_OM_DOCUMENT_GET_ROOT_ELEMENT(document,env) \
        ((document)->ops->get_root_element(document,env))

#define AXIS2_OM_DOCUMENT_SET_ROOT_ELEMENT(document,env,om_node) \
        ((document)->ops->set_root_element(document,env,om_node))       

#define AXIS2_OM_DOCUMENT_BUILD_ALL(document,env) \
        ((document)->ops->build_all(document,env))

/** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OM_DOCUMENT_H */
