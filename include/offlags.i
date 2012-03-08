/* This file was automatically generated. Do not modify it.
*/

/** \file G:/ofsystem/include/offlags.i
    Contains the generated flag enumerations
 */

/** \enum Operations that can be performed on a list OFVALUE
 */
enum LIST_OPERATIONS
    {
        /** Get list size
         */
        LIST_SIZE = 0x01,

        /** Get a list element
         */
        LIST_GET = 0x02,

        /** Set a list element
         */
        LIST_SET = 0x03,

        /** Add a list element
         */
        LIST_ADD = 0x04,

        /** Insert a list element
         */
        LIST_INSERT = 0x05,

        /** Delete a list element
         */
        LIST_DELETE = 0x06
    };

/** \enum Flags for OFFile */
enum OFFILE_OPEN_FLAGS
    {
        /** Always create the file, even if it exists.
         */
        OFFILE_CREATE_ALWAYS = 1,

        /** Attempt to open an existing file. Fails if the file does not exist.
         */
        OFFILE_OPEN_EXISTING = 2,

        /** Always succeed in opening (excluding permission issues). If the file does not exist, create it.
         */
        OFFILE_OPEN_ALWAYS = 4,

        /** Open the file for reading and writing.
         */
        OFFILE_READWRITE = 8,

        /** Open the file for reading only.
         */
        OFFILE_READONLY = 16,

        /** Open the file for writing only.
         */
        OFFILE_WRITEONLY = 32,

        /** Open a temporary file.
         */
        OFFILE_TEMPORARY = 64,

        /** Open for sharing.
         */
        OFFILE_SHAREALL = 128
    };

/** \enum OFFile positioning flags
 */
enum OFFILE_POSITION_FLAGS
    {
        /** Set the file position relative to the beginning of the file.
         */
        OFFILE_POSITION_BEGIN = 1,

        /** Set the file position relative to the end of the file.
         */
        OFFILE_POSITION_END = 2,

        /** Set the file position relative to the current file position.
         */
        OFFILE_POSITION_CURRENT = 3
    };

/** \enum File types
 */
enum OFFILE_FILETYPES
    {
        /** File is a normal file.
         */
        OFFILE_FILETYPE_FILE = 1,

        /** File is a directory.
         */
        OFFILE_FILETYPE_DIR = 2
    };

/** \enum Possible OFVALUE types */
enum OFVALUE_TYPES
    {
        /** integer
         */
        typeInteger = 1,

        /** boolean
         */
        typeBoolean,

        /** string
         */
        typeString,

        /** float
         */
        typeFloat,

        /** datetime
         */
        typeOFDateTime,

        /** identity
         */
        typeIdentity,

        /** binary
         */
        typeBinary
    };

/** \enum General errors */
enum OFERROR
    {
        /** Success.
         */
        ERR_SUCCESS = 1,

        /** Invalid protocol/method.
         */
        ERR_INVAL_PROTMETH,

        /** Compilation error(s).
         */
        ERR_COMPILE_ERROR,

        /** Unknown class.
         */
        ERR_INVAL_CLASS,

        /** Unknown object.
         */
        ERR_INVAL_OBJECT,

        /** Error connecting to the server.
         */
        ERR_PROXY_CONNECT,

        /** Internal execution failure.
         */
        ERR_INT_EXEC_FAIL,

        /** Access denied.
         */
        ERR_ACCDEN,

        /** Entity (class or object) already exists.
         */
        ERR_ENTITY_EXIST,

        /** Entity (class or object) not found.
         */
        ERR_ENTITY_NOTFOUND,

        /** Object storage is busy.
         */
        ERR_OBJSTORE_BUSY,

        /** Function not implemented.
         */
        ERR_NOTIMPL,

        /** Internal DBMS error.
         */
        ERR_INT_DBMS,

        /** Internal active object map error.
         */
        ERR_AOM_INT,

        /** Flagged success.
         */
        ERR_SUCCESS_WITH_INFO,

        /** Internal execution assert.
         */
        ERR_INT_EXEC_ASSERT,

        /** Division by zero.
         */
        ERR_DIVIDE_BY_ZERO,

        /** Execution stack underflow.
         */
        ERR_EXEC_STACK_UNDER,

        /** Internal active object error.
         */
        ERR_AOM_OBJECT_INT,

        /** Undefined variable in execution.
         */
        ERR_EXEC_UNDEF_VAR,

        /** Error opening file.
         */
        ERR_FILE_OPEN_ERR,

        /** A required symbol was not found in the library.
         */
        ERR_LIB_NOSYM,

        /** An error occured communicating with a CLM.
         */
        ERR_CLM_ERROR,

        /** Method parameter count mismatch.
         */
        ERR_METH_PARAM_COUNT,

        /** Method invalid within class.
         */
        ERR_INVAL_METHOD,

        /** Property not found.
         */
        ERR_INVAL_PROPERTY,

        /** A required header field was not found.
         */
        ERR_FIELD_NOTEXIST,

        /** The named instance does not exist.
         */
        ERR_INST_NOTEXIST,

        /** The named instance is already running.
         */
        ERR_INST_ALREADY_RUN,

        /** The named instance is not running.
         */
        ERR_INST_NOT_RUN,

        /** The named instance is a system instance.
         */
        ERR_SYSTEM_INSTANCE,

        /** The class list could not be retrieved from a library.
         */
        ERR_LIB_NOCLASSLIST,

        /** A general error occured while dealing with a server library.
         */
        ERR_LIB_ERROR,

        /** The library did not create the requested class.
         */
        ERR_LIB_NOT_CREATE_CLASS,

        /** The current operation cannot be performed on object's that have children.
         */
        ERR_OBJECT_HAS_CHILDREN,

        /** The current operation cannot be performed on class' that have children.
         */
        ERR_CLASS_HAS_CHILDREN,

        /** You cannot destroy the root class or object.
         */
        ERR_CANT_DESTROY_ROOT,

        /** You cannot destroy a system object.
         */
        ERR_CANT_DESTROY_SYSTEM_OBJECT,

        /** You cannot destroy a system class.
         */
        ERR_CANT_DESTROY_SYSTEM_CLASS,

        /** The supplied library alias is invalid.
         */
        ERR_INVAL_LIB_ALIAS,

        /** The sub-function was not recognized.
         */
        ERR_INVAL_SUB_FUNCTION,

        /** The specified function id was invalid.
         */
        ERR_INVAL_COLONEL_FUNCTION,

        /** The data identity specified does not exist.
         */
        ERR_INVAL_DATA_ID,

        /** An error occurred within Elvis.
         */
        ERR_ELVIS_ERROR,

        /** Invalid username or password.
         */
        ERR_INVAL_USERPASS,

        /** The specified session is invalid.
         */
        ERR_SESSION_INVALID,

        /** The network has failed in transaction.
         */
        ERR_NETWORK_FAILED,

        /** The transaction has been aborted due to deadlock.
         */
        ERR_DEADLOCK_ABORT,

        /** The parent given is not a valid object.
         */
        ERR_INVAL_PARENT,

        /** The name given is already in use.
         */
        ERR_INVAL_NAME,

        /** The handle supplied is invalid.
         */
        ERR_INVAL_FILE_HANDLE,

        /** The server has encountered a socket error.
         */
        ERR_OFSOCKET_ERROR,

        /** Internal OpenFabric Object Repository error.
         */
        ERR_INT_OFOR,

        /** The CLM could not retrieve the OFAP message.
         */
        ERR_CLM_NO_OFAP_MSG,

        /** The CLM could not perform the XSL transformation.
         */
        ERR_CLM_XSL_TRANSFORM_FAILED,

        /** The socket timed out while sending or receiving.
         */
        ERR_SOCKET_TIMEOUT,

        /** The server could not be located.
         */
        ERR_UNLOCATABLE,

        /** Could not connect or disconnect from LDAP server.
         */
        ERR_LDAP_CONNECTION,

        /** LDAP entry not found.
         */
        ERR_LDAP_ENTRY,

        /** LDAP attribute not found.
         */
        ERR_LDAP_ATTRIBUTE,

        /** Could not add entry to LDAP server.
         */
        ERR_LDAP_ADD,

        /** Could not delete entry from LDAP server.
         */
        ERR_LDAP_DELETE,

        /** Could not modify LDAP entry.
         */
        ERR_LDAP_MODIFY,

        /** Could not create new id.
         */
        ERR_NEW_ID,

        /** Entity could not be authenticated.
         */
        ERR_AUTHENTICATION_FAILURE,

        /** Could not load certificate serial number or rename file.
         */
        ERR_SERIAL_FILE,

        /** Memory allocation failure.
         */
        ERR_MEMORY_ALLOCATION,

        /** Could not read certificate request.
         */
        ERR_CERT_REQUEST,

        /** Certificate request has wrong format or signature.
         */
        ERR_CERT_REQUEST_FORMAT,

        /** Email address in certificate request has wrong format.
         */
        ERR_EMAIL_FORMAT,

        /** The certificate request contains characters that are illegal for the ASN.1 type.
         */
        ERR_ILLEGAL_CHARACTERS,

        /** A required field in certificate request is missing.
         */
        ERR_REQUIRED_FIELD_MISSING,

        /** A matching field in certificate request is missing.
         */
        ERR_MATCHING_FIELD_MISSING,

        /** Missing field in CA certificate.
         */
        ERR_CA_FIELD_MISSING,

        /** Required field in certificate request does not match CA certificate.
         */
        ERR_MATCHING_FIELD,

        /** An invalid type exists in the CA policy.
         */
        ERR_INVALID_TYPE_IN_POLICY,

        /** The user name already exists.
         */
        ERR_USER_NAME_EXISTS,

        /** The user name does not exist.
         */
        ERR_USER_NAME_DOES_NOT_EXIST,

        /** A user name is required for this user.
         */
        ERR_USER_NAME_REQUIRED,

        /** The server already exists.
         */
        ERR_ID_EXISTS,

        /** The server does not exist.
         */
        ERR_ID_DOES_NOT_EXIST,

        /** Certificate has wrong format.
         */
        ERR_INVALID_CERT,

        /** Could not sign data.
         */
        ERR_SIGNING_FAILED,

        /** The certificate has expired.
         */
        ERR_CERT_EXPIRED,

        /** The certificate has been revoked.
         */
        ERR_CERT_REVOKED,

        /** A certificate is required for this user.
         */
        ERR_CERT_REQUIRED,

        /** The signature supplied with this user token is invalid.
         */
        ERR_INVALID_SIGNATURE,

        /** The reference count could not be updated.
         */
        ERR_REF_COUNT_CHANGE,

        /** Could not connect to server.
         */
        ERR_CONNECTION_ERROR,

        /** The user or server has not been authenticated yet.
         */
        ERR_NOT_AUTHENTICATED,

        /** The address or port numbers for the registry could not be found.
         */
        ERR_REGISTRY_INFO,

        /** Could not contact the registry.
         */
        ERR_NO_CONTACT_REGISTRY,

        /** Could not contact the sentinel server.
         */
        ERR_NO_CONTACT_SENTINEL,

        /** Could not contact the access manager.
         */
        ERR_NO_CONTACT_ACCESS,

        /** Could not contact the required component.
         */
        ERR_NO_CONTACT_COMPONENT,

        /** Invalid server (from registry)
         */
        ERR_REGISTRY_INVAL_SERVER,

        /** Invalid user (from registry)
         */
        ERR_REGISTRY_INVAL_USER,

        /** The server name to connect to has not been specified.
         */
        ERR_NAME_NOT_SET,

        /** The authentication token is invalid.
         */
        ERR_INVALID_TOKEN,

        /** The action is not a legal operation for the registry or authentication server.
         */
        ERR_ILLEGAL_ACTION,

        /** A field in the message is illegal.
         */
        ERR_ILLEGAL_FIELD,

        /** Could not create private key.
         */
        ERR_PRIVATE_KEY,

        /** Could not write private key.
         */
        ERR_WRITE_PRIVATE_KEY,

        /** Could not sign certificate request.
         */
        ERR_SIGN_REQ,

        /** Could not write certificate request.
         */
        ERR_WRITE_REQ,

        /** Could not set certificate request version.
         */
        ERR_REQ_VERSION,

        /** Could not add subject field to certificate request.
         */
        ERR_ADD_SUBJECT_FIELD,

        /** Could not write certificate.
         */
        ERR_WRITE_CERT,

        /** Could not set certificate or private key for socket.
         */
        ERR_CERT_OR_PRIV_KEY,

        /** Could not replace the common name in certificate.
         */
        ERR_REPLACE_NAME,

        /** The address or port numbers for the server are invalid.
         */
        ERR_ADDRESS,

        /** The message IP address does not match the IP address for this session.
         */
        ERR_IP_ADDRESS,

        /** Certificate extension cannot be added.
         */
        ERR_CANNOT_ADD_EXTENSION,

        /** User must be authenticated.
         */
        ERR_AUTHENTICATION_REQUIRED,

        /** Could not verify authentication token signature.
         */
        ERR_VERIFY_FAILED,

        /** Could not find responder.
         */
        ERR_RESPONDER_NOT_EXISTS,

        /** The responder could not handle the method.
         */
        ERR_INVALID_RESP_METHOD,

        /** Could not access config file.
         */
        ERR_CONFIG_FILE,

        /** Could not establish SSL connection with server.
         */
        ERR_SSL_CONNECT,

        /** Server certificate is invalid or has wrong server id.
         */
        ERR_SSL_CERT,

        /** Server private key is invalid or could not be loaded.
         */
        ERR_SSL_PRIVKEY,

        /** Server certificate and key do not match.
         */
        ERR_SSL_CERTKEY_MISMATCH,

        /** An error occured accessing the serial number file.
         */
        ERR_SSL_SERIALFILE_ERROR,

        /** Invalid task index.
         */
        ERR_SCHEDULER_INVAL_TASK,

        /** SSL Connection required for specified operation.
         */
        ERR_SSL_CONNECTION_REQUIRED,

        /** No identities available for requested pool.
         */
        ERR_REG_NO_ID_LEFT,

        /** Invalid licence file format.
         */
        ERR_INVAL_LIC_FORMAT,

        /** Distributed transaction not commited on all machines/
         */
        ERR_COMMIT_INCOMPLETE,

        /** Internal use only - transaction must wait for another.
         */
        ERR_WAIT_FOR,

        /** Internal use only - remote field not on remote machine.
         */
        ERR_NOT_LOCAL,

        /** Internal use only - server has reached unreachable code.
         */
        ERR_SICK,

        /** Invalid network request received by repository.
         */
        ERR_INVAL_REQUEST,

        /** Bad link object.
         */
        ERR_BAD_LINK,

        /** Transaction not a valid one.
         */
        ERR_INVAL_TRANSACTION,

        /** Link is to a local object.
         */
        ERR_LOCAL_LINK,

        /** Proxy internal error.
         */
        ERR_PROXY_INTERNAL,

        /** The entity does not contain the requested ACL.
         */
        ERR_EMPTY_ACL,

        /** Invalid execution context.
         */
        ERR_INVAL_EXEC_CONTEXT,

        /** Transaction being used is no longer running.
         */
        ERR_TRANSACTION_DEAD,

        /** Unknown acl management function.
         */
        ERR_UNKNOWN_ACLMAN_FUNC,

        /** Error parsing input xml file.
         */
        ERR_XML_ERROR_PARSING_INPUT_FILE,

        /** Error parsing WSDL file.
         */
        ERR_XML_ERROR_PARSING_WSDL_FILE,

        /** Error parsing WSDL IMPORT file.
         */
        ERR_XML_ERROR_PARSING_WSDL_IMPORT_FILE,

        /** Error parsing XSLT stylesheet.
         */
        ERR_XML_ERROR_PARSING_XSLT_STYLESHEET,

        /** Unable to retrieve WSDL file.
         */
        ERR_XML_CANNOT_RETRIEVE_WSDL_FILE,

        /** Error perfoming xslt transform.
         */
        ERR_XSLT_TRANSFORM,

        /** Error related to perfoming xslt transform.
         */
        ERR_XSLT_TRANSFORM_RELATED,

        /** Unable to retrieve data from object.
         */
        ERR_SOAP_ERROR_LOADING_OBJECT,

        /** Error parsing WSDL complex types.
         */
        ERR_WSDL_ERROR_PARSING_COMPLEX_TYPES,

        /** Compile of complex Types failed.
         */
        ERR_WSDL_CXT_COMPILE_FAILED,

        /** Compile of WSDL Class failed.
         */
        ERR_WSDL_CLASS_COMPILE_FAILED,

        /** Unable to create class from WSDL.
         */
        ERR_WSDL_CANNOT_CREATE_CLASS,

        /** Missing Document or default XML namespace in WSDL document.
         */
        ERR_WSDL_CANNOT_FIND_DEFAULT_NAMESPACE_IN_WSDL_FILE,

        /** Missing default XMLSchema namespace in WSDL document.
         */
        ERR_WSDL_CANNOT_FIND_XMLSCHEMA_NAMESPACE_IN_WSDL_FILE,

        /** Missing default XML namespace in WSDL IMPORT document.
         */
        ERR_WSDL_CANNOT_FIND_DEFAULT_NAMESPACE_IN_WSDL_IMPORT_FILE,

        /** Missing default XMLSchema namespace in WSDL IMPORT document.
         */
        ERR_WSDL_CANNOT_FIND_XMLSCHEMA_NAMESPACE_IN_WSDL_IMPORT_FILE,

        /** Missing namespace attribute in WSDL IMPORT tag.
         */
        ERR_WSDL_CANNOT_FIND_NAMESPACE_IN_WSDL_IMPORT_TAG,

        /** Missing location attribute in WSDL IMPORT tag.
         */
        ERR_WSDL_CANNOT_FIND_LOCATION_IN_WSDL_IMPORT_TAG,

        /** Error adding child node from WSDL IMPORT document.
         */
        ERR_WSDL_ERROR_ADDING_CHILD_NODE_FROM_WSDL_IMPORT_FILE,

        /** Nothing found to import in WSDL IMPORT document.
         */
        ERR_WSDL_NOTHING_TO_IMPORT_FROM_WSDL_IMPORT_FILE,

        /** Missing Service name in WSDL document.
         */
        ERR_WSDL_CANNOT_FIND_SERVICE_NAME,

        /** Missing Ports in WSDL Document.
         */
        ERR_WSDL_CANNOT_FIND_ANY_PORTS,

        /** Missing binding attribute for Port in WSDL document.
         */
        ERR_WSDL_CANNOT_FIND_BINDING_ATTRIBUTE_FOR_PORT,

        /** Missing location for Port in WSDL document.
         */
        ERR_WSDL_CANNOT_FIND_LOCATION_FOR_PORT,

        /** Missing Binding for Port in WSDL document.
         */
        ERR_WSDL_CANNOT_FIND_BINDING_TYPE,

        /** Missing Binding Operations in WSDL document.
         */
        ERR_WSDL_CANNOT_FIND_BINDING_OPERATIONS,

        /** Missing soapAction in WSDL Document.
         */
        ERR_WSDL_MISSING_SOAP_ACTION,

        /** Missing portType Operations in WSDL document.
         */
        ERR_WSDL_CANNOT_FIND_PORTTYPE_OPERATIONS,

        /** Missing name attribute for element or complexType in WSDL document.
         */
        ERR_WSDL_MISSING_ATTRIBUTE_FOR_COMPLEX_TYPE,

        /** Error adding container class.
         */
        ERR_WSDL_ERROR_ADDING_CONTAINER_CLASS,

        /** Error adding base class.
         */
        ERR_WSDL_ERROR_ADDING_BASE_CLASS,

        /** Error adding complexType class.
         */
        ERR_WSDL_ERROR_ADDING_COMPLEXTYPE_CLASS,

        /** Cannot parse complex type names file.
         */
        ERR_WSDL_ERROR_PARSING_COMPLEXTYPE_NAMES_FILE,

        /** Cannot parse complex type slicer file.
         */
        ERR_WSDL_ERROR_PARSING_COMPLEXTYPE_SLICER_FILE,

        /** Failed to resolve method return complexType reference to an openFabric class.
         */
        ERR_WSDL_FAILED_TO_RESOLVE_METHOD_RETURN_COMPLEXTYPE_REFERENCE,

        /** Failed to resolve parameter complexType reference to an openFabric class.
         */
        ERR_WSDL_FAILED_TO_RESOLVE_PARAMETER_COMPLEXTYPE_REFERENCE,

        /** ailed to resolve method reference.
         */
        ERR_WSDL_FAILED_TO_RESOLVE_METHOD,

        /** Failed to resolve property complexType reference to an openFabric class.
         */
        ERR_WSDL_FAILED_TO_RESOLVE_PROPERTY_COMPLEXTYPE_REFERENCE,

        /** Failed to complete XSLT transform.
         */
        ERR_WSDL_XSLT_TRANSFORM_FAILED,

        /** Failed to recognise type of WSDL file.
         */
        ERR_WSDL_FAILED_TO_DETERMINE_TYPE_OF_WSDL_FILE,

        /** Failed to obtain ActiveClassGuard.
         */
        ERR_WSDL_FAILED_TO_OBTAIN_CLSGUARD,

        /** Cannot parse wsdl complexType preprocess stylesheet.
         */
        ERR_WSDL_ERROR_PARSING_WSDLCXTPREPROCESS_SLICER_FILE,

        /** Cannot parse wsdl preprocess stylesheet.
         */
        ERR_WSDL_ERROR_PARSING_WSDLPREPROCESS_SLICER_FILE,

        /** Cannot parse A NULL WSDL URI.
         */
        ERR_WSDL_NO_URI,

        /** Applying wsdl_analyse stylesheet returns NULL.
         */
        ERR_ERROR_APPLYING_WSDL_ANALYSE_STYLESHEET,

        /** Applying wsdl_complexType_analyse stylesheet returns NULL.
         */
        ERR_ERROR_APPLYING_WSDL_CXT_ANALYSE_STYLESHEET,

        /** Applying wsdl_cxt_slicer stylesheet returns NULL.
         */
        ERR_ERROR_APPLYING_WSDL_CXT_SLICER_STYLESHEET,

        /** Applying wsdl_imports_analyse stylesheet returns NULL.
         */
        ERR_ERROR_APPLYING_WSDL_IMPORTS_ANALYSE_STYLESHEET,

        /** Applying wsdl_cxt_names stylesheet returns NULL.
         */
        ERR_ERROR_APPLYING_WSDL_CXT_NAMES_STYLESHEET,

        /** Applying wsdl_preprocess stylesheet returns NULL.
         */
        ERR_ERROR_APPLYING_PREPROCESS_STYLESHEET,

        /** Applying wsdl_cxt_preprocess stylesheet returns NULL.
         */
        ERR_ERROR_APPLYING_CXT_PREPROCESS_STYLESHEET,

        /** The message did not contain a relevant access token.
         */
        ERR_ACCESS_TOKEN_NOT_FOUND,

        /** Failed to re-add class to repository after type resolution.
         */
        ERR_WSDL_CANNOT_RE_ADD_CLASS,

        /** Call to remote SOAP server failed.
         */
        ERR_SOAP_CALL_TO_REMOTE_SERVER_FAILED,

        /** Remote SOAP server returned invalid response.
         */
        ERR_SOAP_REMOTE_SERVER_RETURNED_INVALID_RESPONSE,

        /** Missing operation namespace.
         */
        ERR_SOAP_FAILED_TO_FIND_OPERATION_NAMESPACE,

        /** Failed to parse SOAP server response.
         */
        ERR_SOAP_FAILED_TO_PARSE_RESPONSE,

        /** ailed to resolve operation name returned by SOAP server.
         */
        ERR_SOAP_FAILED_TO_RESOLVE_OPERATION_NAME,

        /** Failed to find SOAP Action.
         */
        ERR_SOAP_FAILED_TO_FIND_SOAP_ACTION,

        /** Failed to find location.
         */
        ERR_SOAP_FAILED_TO_FIND_LOCATION,

        /** Failed to find namespace for operation.
         */
        ERR_SOAP_FAILED_TO_FIND_NAMESPACE_FOR_OPERATION,

        /** Failed to find return value(s) in SOAP response.
         */
        ERR_SOAP_FAILED_TO_FIND_RESPONSE_RETURN_VALUE,

        /** SOAP Fault Code returned.
         */
        ERR_SOAP_SOAP_FAULT_RETURNED,

        /** The supplied token has expired.
         */
        ERR_TOKEN_EXPIRED,

        /** The address supplied (OFProxy/OFSocket) is invalid.
         */
        ERR_ADDRESS_INVALID,

        /** The supplied queue is invalid.
         */
        ERR_INVAL_QUEUE,

        /** There are no available messages.
         */
        ERR_NO_MESSAGES,

        /** The supplied listener is invalid.
         */
        ERR_INVAL_LISTENER,

        /** The supplied listener is already registered for the supplied subject.
         */
        ERR_ALREADY_REGISTERED,

        /** Invalid or unsupported event.
         */
        ERR_INVAL_EVENT,

        /** Supplied value is out of a required range.
         */
        ERR_OUT_OF_RANGE,

        /** The current format is invalid for the specified operation.
         */
        ERR_INVAL_FORMAT,

        /** The incoming message is malformed.
         */
        ERR_OFAP_MSG_MALFORMED,

        /** The supplied index is invalid.
         */
        ERR_INVALID_INDEX,

        /** System error - Operation not permitted
         */
        ERR_EPERM,

        /** System error - No such file or directory
         */
        ERR_ENOENT,

        /** System error - No such process
         */
        ERR_ESRCH,

        /** System error - Interrupted system call
         */
        ERR_EINTR,

        /** System error - I/O error
         */
        ERR_EIO,

        /** System error - No such device or address
         */
        ERR_ENXIO,

        /** System error - Arg list too long
         */
        ERR_E2BIG,

        /** System error - Exec format error
         */
        ERR_ENOEXEC,

        /** System error - Bad file number
         */
        ERR_EBADF,

        /** System error - No child processes
         */
        ERR_ECHILD,

        /** System error - Try again
         */
        ERR_EAGAIN,

        /** System error - Out of memory
         */
        ERR_ENOMEM,

        /** System error - Permission denied
         */
        ERR_EACCES,

        /** System error - Bad address
         */
        ERR_EFAULT,

        /** System error - Block device required
         */
        ERR_ENOTBLK,

        /** System error - Device or resource busy
         */
        ERR_EBUSY,

        /** System error - File exists
         */
        ERR_EEXIST,

        /** System error - Cross-device link
         */
        ERR_EXDEV,

        /** System error - No such device
         */
        ERR_ENODEV,

        /** System error - Not a directory
         */
        ERR_ENOTDIR,

        /** System error - Is a directory
         */
        ERR_EISDIR,

        /** System error - Invalid argument
         */
        ERR_EINVAL,

        /** System error - File table overflow
         */
        ERR_ENFILE,

        /** System error - Too many open files
         */
        ERR_EMFILE,

        /** System error - Not a typewriter
         */
        ERR_ENOTTY,

        /** System error - Text file busy
         */
        ERR_ETXTBSY,

        /** System error - File too large
         */
        ERR_EFBIG,

        /** System error - No space left on device
         */
        ERR_ENOSPC,

        /** System error - Illegal seek
         */
        ERR_ESPIPE,

        /** System error - Read-only file system
         */
        ERR_EROFS,

        /** System error - Too many links
         */
        ERR_EMLINK,

        /** System error - Broken pipe
         */
        ERR_EPIPE,

        /** System error - Math argument out of domain of func
         */
        ERR_EDOM,

        /** System error - Math result not representable
         */
        ERR_ERANGE,

        /** System error - Resource deadlock would occur
         */
        ERR_EDEADLK,

        /** System error - File name too long
         */
        ERR_ENAMETOOLONG,

        /** System error - No record locks available
         */
        ERR_ENOLCK,

        /** System error - Function not implemented
         */
        ERR_ENOSYS,

        /** System error - Directory not empty
         */
        ERR_ENOTEMPTY,

        /** System error - Too many symbolic links encountered
         */
        ERR_ELOOP,

        /** System error - Operation would block
         */
        ERR_EWOULDBLOCK,

        /** System error - No message of desired type
         */
        ERR_ENOMSG,

        /** System error - Identifier removed
         */
        ERR_EIDRM,

        /** System error - Channel number out of range
         */
        ERR_ECHRNG,

        /** System error - Level not synchronized
         */
        ERR_EL2NSYNC,

        /** System error - Level halted
         */
        ERR_EL3HLT,

        /** System error - Level reset
         */
        ERR_EL3RST,

        /** System error - Link number out of range
         */
        ERR_ELNRNG,

        /** System error - Protocol driver not attached
         */
        ERR_EUNATCH,

        /** System error - No CSI structure available
         */
        ERR_ENOCSI,

        /** System error - Level 2 halted
         */
        ERR_EL2HLT,

        /** System error - Invalid exchange
         */
        ERR_EBADE,

        /** System error - Invalid request descriptor
         */
        ERR_EBADR,

        /** System error - Exchange full
         */
        ERR_EXFULL,

        /** System error - No anode
         */
        ERR_ENOANO,

        /** System error - Invalid request code
         */
        ERR_EBADRQC,

        /** System error - Invalid slot
         */
        ERR_EBADSLT,

        /** System error - Dead lock
         */
        ERR_EDEADLOCK,

        /** System error - Bad font file format
         */
        ERR_EBFONT,

        /** System error - Device not a stream
         */
        ERR_ENOSTR,

        /** System error - No data available
         */
        ERR_ENODATA,

        /** System error - Timer expired
         */
        ERR_ETIME,

        /** System error - Out of streams resources
         */
        ERR_ENOSR,

        /** System error - Machine is not on the network
         */
        ERR_ENONET,

        /** System error - Package not installed
         */
        ERR_ENOPKG,

        /** System error - Object is remote
         */
        ERR_EREMOTE,

        /** System error - Link has been severed
         */
        ERR_ENOLINK,

        /** System error - Advertise error
         */
        ERR_EADV,

        /** System error - Srmount error
         */
        ERR_ESRMNT,

        /** System error - Communication error on send
         */
        ERR_ECOMM,

        /** System error - Protocol error
         */
        ERR_EPROTO,

        /** System error - Multihop attempted
         */
        ERR_EMULTIHOP,

        /** System error - RFS specific error
         */
        ERR_EDOTDOT,

        /** System error - Not a data message
         */
        ERR_EBADMSG,

        /** System error - Value too large for d data type
         */
        ERR_EOVERFLOW,

        /** System error - Name not unique on network
         */
        ERR_ENOTUNIQ,

        /** System error - File descriptor in bad state
         */
        ERR_EBADFD,

        /** System error - Remote address changed
         */
        ERR_EREMCHG,

        /** System error - Can not access a needed shared library
         */
        ERR_ELIBACC,

        /** System error - Accessing a corrupted shared library
         */
        ERR_ELIBBAD,

        /** System error - .lib section in a.out corrupted
         */
        ERR_ELIBSCN,

        /** System error - Attempting to link in too many shared libraries
         */
        ERR_ELIBMAX,

        /** System error - Cannot exec a shared library directly
         */
        ERR_ELIBEXEC,

        /** System error - Illegal byte sequence
         */
        ERR_EILSEQ,

        /** System error - Interrupted system call should be restarted
         */
        ERR_ERESTART,

        /** System error - Streams pipe error
         */
        ERR_ESTRPIPE,

        /** System error - Too many users
         */
        ERR_EUSERS,

        /** System error - Socket operation on non-socket
         */
        ERR_ENOTSOCK,

        /** System error - Destination address required
         */
        ERR_EDESTADDRREQ,

        /** System error - Message too long
         */
        ERR_EMSGSIZE,

        /** System error - Protocol wrong type for socket
         */
        ERR_EPROTOTYPE,

        /** System error - Protocol not available
         */
        ERR_ENOPROTOOPT,

        /** System error - Protocol not supported
         */
        ERR_EPROTONOSUPPORT,

        /** System error - Socket type not supported
         */
        ERR_ESOCKTNOSUPPORT,

        /** System error - Operation not supported on transport endpoint
         */
        ERR_EOPNOTSUPP,

        /** System error - Protocol family not supported
         */
        ERR_EPFNOSUPPORT,

        /** System error - Address family not supported by protocol
         */
        ERR_EAFNOSUPPORT,

        /** System error - Address already in use
         */
        ERR_EADDRINUSE,

        /** System error - Cannot assign requested address
         */
        ERR_EADDRNOTAVAIL,

        /** System error - Network is down
         */
        ERR_ENETDOWN,

        /** System error - Network is unreachable
         */
        ERR_ENETUNREACH,

        /** System error - Network dropped connection because of reset
         */
        ERR_ENETRESET,

        /** System error - Software caused connection abort
         */
        ERR_ECONNABORTED,

        /** System error - Connection reset by peer
         */
        ERR_ECONNRESET,

        /** System error - No buffer space available
         */
        ERR_ENOBUFS,

        /** System error - Transport endpoint is already connected
         */
        ERR_EISCONN,

        /** System error - Transport endpoint is not connected
         */
        ERR_ENOTCONN,

        /** System error - Cannot send after transport endpoint shutdown
         */
        ERR_ESHUTDOWN,

        /** System error - Too many references         */
        ERR_ETOOMANYREFS,

        /** System error - Connection timed out
         */
        ERR_ETIMEDOUT,

        /** System error - Connection refused
         */
        ERR_ECONNREFUSED,

        /** System error - Host is down
         */
        ERR_EHOSTDOWN,

        /** System error - No route to host
         */
        ERR_EHOSTUNREACH,

        /** System error - Operation already in progress
         */
        ERR_EALREADY,

        /** System error - Operation now in progress
         */
        ERR_EINPROGRESS,

        /** System error - Stale NFS file handle
         */
        ERR_ESTALE,

        /** System error - Structure needs cleaning
         */
        ERR_EUCLEAN,

        /** System error - Not a XENIX named type file
         */
        ERR_ENOTNAM,

        /** System error - No XENIX semaphores available
         */
        ERR_ENAVAIL,

        /** System error - Is a named type file
         */
        ERR_EISNAM,

        /** System error - Remote I/O error
         */
        ERR_EREMOTEIO,

        /** System error - Quota exceeded
         */
        ERR_EDQUOT,

        /** System error - No medium found
         */
        ERR_ENOMEDIUM,

        /** System error - Wrong medium type
         */
        ERR_EMEDIUMTYPE,

        /** Unknown (unmappable) system error
         */
        ERR_UNKNOWN_SYSTEM_ERROR,

        /** Invalid socket value
         */
        ERR_INVAL_SOCKET,

        /** The user identities do not match.
         */
        ERR_USER_ID_MISMATCH,

        /** Invalid public key
         */
        ERR_INVAL_PUBKEY,

        /** Invalid entity or right
         */
        ERR_INVAL_ENTITY_RIGHT,

        /** Transaction is still running
         */
        ERR_TRANSACTION_ALIVE,

        /** Entity is not a user
         */
        ERR_NOT_USER,

        /** Entity is not a role
         */
        ERR_NOT_ROLE,

        /** The list already contains an entry with that value
         */
        ERR_LIST_DUPLICATE_NOT_ALLOWED,

        /** A specified holder selection method is invalid
         */
        ERR_WF_HOLDER_SELECTION_METHOD_NOT_FOUND,

        /** Abort transaction
         */
        ERR_ABORT_TRANSACTION,

        /** Commit transaction
         */
        ERR_COMMIT_TRANSACTION,

        /** Transaction aborted
         */
        ERR_TRANSACTION_ABORTED,

        /** Transaction committed.
         */
        ERR_TRANSACTION_COMMITED,

        /** Object or entity is read only (its a slave embedded object)
         */
        ERR_READ_ONLY_SLAVE,

        /** No data was present when it was expected.
         */
        ERR_DATA_NOT_PRESENT,

        /** The specified object is not embedded.
         */
        ERR_OBJECT_NOT_EMBEDDED,

        /** The internal property specified is invalid (never set)
         */
        ERR_INTERNAL_PROP_INVALID,

        /** The assign attempted is using mismatched types.
         */
        ERR_INVALID_RUNTIME_ASSIGN,

        /** Internal class implementation could not be found.
         */
        ERR_INTERNAL_CLASS_NOTIMPL,

        /** The supplied component is invalid.
         */
        ERR_INVALID_COMPONENT,

        /** A type mismatch has occurred.
         */
        ERR_RUNTIME_TYPE_MISMATCH,

        /** A debug function was attempted on a non debug session.
         */
        ERR_NOT_DEBUGGING,

        /** The local server is not registered with the registry.
         */
        ERR_SRV_NOT_REGISTERED,

        /** The supplied ticket is not in a valid format.
         */
        ERR_SERVERTICKET_FORMAT,

        /** Invalid cache register identity supplied.
         */
        ERR_INVAL_CACHE_REGISTER_ID,

        /** Invalid cache item (index or name).
         */
        ERR_INVAL_CACHE_ITEM,

        /** The signing server is not in our trusted list.
         */
        ERR_SERVER_NOT_TRUSTED,

        /** A blocking operation was interrupted by a call to WSACancelBlockingCall (WIN32-10004)
         */
        ERR_WSAEINTR_10004,

        /** The file handle supplied is not valid. (WIN32-10009)
         */
        ERR_WSAEBADF_10009,

        /** An attempt was made to access a socket in a way forbidden by its access permissions. (WIN32-10013)
         */
        ERR_WSAEACCES_10013,

        /** The system detected an invalid pointer address in attempting to use a pointer argument in a call. (WIN32-10014)
         */
        ERR_WSAEFAULT_10014,

        /** An invalid argument was supplied (WIN32-10022)
         */
        ERR_WSAEINVAL_10022,

        /** Too many open sockets. (WIN32-10024)
         */
        ERR_WSAEMFILE_10024,

        /** A non-blocking socket operation could not be completed immediately. (WIN32-10035)
         */
        ERR_WSAEWOULDBLOCK_10035,

        /** A blocking operation is currently executing. (WIN32-10036)
         */
        ERR_WSAEINPROGRESS_10036,

        /** An operation was attempted on a non-blocking socket that already had an operation in progress. (WIN32-10037)
         */
        ERR_WSAEALREADY_10037,

        /** An operation was attempted on something that is not a socket. (WIN32-10038)
         */
        ERR_WSAENOTSOCK_10038,

        /** A required address was omitted from an operation on a socket. (WIN32-10039)
         */
        ERR_WSAEDESTADDRREQ_10039,

        /** A message sent on a datagram socket was larger than the internal message buffer or some other network limit, or the buffer used to receive a datagram into was smaller than the datagram itself. (WIN32-10040)
         */
        ERR_WSAEMSGSIZE_10040,

        /** A protocol was specified in the socket function call that does not support the semantics of the socket type requested. (WIN32-10041)
         */
        ERR_WSAEPROTOTYPE_10041,

        /** An unknown, invalid, or unsupported option or level was specified in a getsockopt or setsockopt call. (WIN32-10042)
         */
        ERR_WSAENOPROTOOPT_10042,

        /** The requested protocol has not been configured into the system, or no implementation for it exists. (WIN32-10043)
         */
        ERR_WSAEPROTONOSUPPORT_10043,

        /** The support for the specified socket type does not exist in this address family. (WIN32-10044)
         */
        ERR_WSAESOCKTNOSUPPORT_10044,

        /** The attempted operation is not supported for the type of object referenced. (WIN32-10045)
         */
        ERR_WSAEOPNOTSUPP_10045,

        /** The protocol family has not been configured into the system or no implementation for it exists. (WIN32-10046)
         */
        ERR_WSAEPFNOSUPPORT_10046,

        /** An address incompatible with the requested protocol was used. (WIN32-10047)
         */
        ERR_WSAEAFNOSUPPORT_10047,

        /** Only one usage of each socket address (protocol/network address/port) is normally permitted. (WIN32-10048)
         */
        ERR_WSAEADDRINUSE_10048,

        /** The requested address is not valid in its context. (WIN32-10049)
         */
        ERR_WSAEADDRNOTAVAIL_10049,

        /** A socket operation encountered a dead network. (WIN32-10050)
         */
        ERR_WSAENETDOWN_10050,

        /** A socket operation was attempted to an unreachable network. (WIN32-10051)
         */
        ERR_WSAENETUNREACH_10051,

        /** The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress. (WIN32-10052)
         */
        ERR_WSAENETRESET_10052,

        /** An established connection was aborted by the software in your host machine. (WIN32-10053)
         */
        ERR_WSAECONNABORTED_10053,

        /** An existing connection was forcibly closed by the remote host. (WIN32-10054)
         */
        ERR_WSAECONNRESET_10054,

        /** An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full. (WIN32-10055)
         */
        ERR_WSAENOBUFS_10055,

        /** A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using a sendto call) no address was supplied. (WIN32-10057)
         */
        ERR_WSAENOTCONN_10057,

        /** A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call. (WIN32-10058)
         */
        ERR_WSAESHUTDOWN_10058,

        /** Too many references to some kernel object. (WIN32-10059)
         */
        ERR_WSAETOOMANYREFS_10059,

        /** Cannot translate name. (WIN32-10062)
         */
        ERR_WSAELOOP_10062,

        /** Name component or name was too long. (WIN32-10063)
         */
        ERR_WSAENAMETOOLONG_10063,

        /** A socket operation failed because the destination host was down. (WIN32-10064)
         */
        ERR_WSAEHOSTDOWN_10064,

        /** A socket operation was attempted to an unreachable host. (WIN32-10065)
         */
        ERR_WSAEHOSTUNREACH_10065,

        /** Cannot remove a directory that is not empty. (WIN32-10066)
         */
        ERR_WSAENOTEMPTY_10066,

        /** A Windows Sockets implementation may have a limit on the number of applications that may use it simultaneously. (WIN32-10067)
         */
        ERR_WSAEPROCLIM_10067,

        /** Ran out of quota. (WIN32-10068)
         */
        ERR_WSAEUSERS_10068,

        /** Ran out of disk quota. (WIN32-10069)
         */
        ERR_WSAEDQUOT_10069,

        /** File handle reference is no longer available. (WIN32-10070)
         */
        ERR_WSAESTALE_10070,

        /** Item is not available locally. (WIN32-10071)
         */
        ERR_WSAEREMOTE_10071,

        /** WSAStartup cannot function at this time because the underlying system it uses to provide network services is currently unavailable. (WIN32-10091)
         */
        ERR_WSASYSNOTREADY_10091,

        /** The Windows Sockets version requested is not supported. (WIN32-10092)
         */
        ERR_WSAVERNOTSUPPORTED_10092,

        /** Either the application has not called WSAStartup, or WSAStartup failed. (WIN32-10093)
         */
        ERR_WSANOTINITIALISED_10093,

        /** The socket is already connected (WIN32-10056).
         */
        ERR_WSAEISCONN,

        /** The connection was refused (WIN32-10061).
         */
        ERR_WSACONNREFUSED,

        /** The connection timed out (WIN32-10060)
         */
        ERR_WSAETIMEDOUT_10060,

        /** The thread is still running.
         */
        ERR_THREAD_RUNNING,

        /** No such thread has been found.
         */
        ERR_THREAD_NOT_FOUND,

        /** The object is foreign or a link.
         */
        ERR_FOREIGN_OBJECT,

        /** The space supplied has no rules.
         */
        ERR_RULE_INVALID_SPACE,

        /** No matching rule for the container and mimetype was found.
         */
        ERR_RULE_NO_MATCH_CONTENT,

        /** No matching rule for the container was found.
         */
        ERR_RULE_NO_MATCH_FOLDER,

        /** No new updates available
         */
        ERR_UPDATES_NON_AVAIL,

        /** Update creation failed.
         */
        ERR_UPDATES_CREATE_FAIL,

        /** Invalid operating system
         */
        ERR_UPDATES_INVAL_OPSYS,

        /** Server or user is already a member of the role
         */
        ERR_ALREADY_ROLE_MEMBER,

        /** SSL_ERROR_ZERO_RETURN.The TLS/SSL connection has been closed.
         */
        ERR_SSL_ERROR_ZERO_RETURN,

        /** SSL_ERROR_WANT_READ. The operation did not complete.
         */
        ERR_SSL_ERROR_WANT_READ,

        /** SSL_ERROR_WANT_WRITE. The operation did not complete.
         */
        ERR_SSL_ERROR_WANT_WRITE,

        /** SSL_ERROR_WANT_CONNECT. The operation did not complete.
         */
        ERR_SSL_ERROR_WANT_CONNECT,

        /** SSL_ERROR_WANT_ACCEPT. The operation did not complete.
         */
        ERR_SSL_ERROR_WANT_ACCEPT,

        /** SSL_ERROR_WANT_X509_LOOKUP. The operation did not complete because an application callback set by SSL_CTX_set_client_cert_cb() has asked to be called again.
         */
        ERR_SSL_ERROR_WANT_X509_LOOKUP,

        /** SSL_ERROR_SYSCALL. Some I/O error occurred. The OpenSSL error queue may contain more information.
         */
        ERR_SSL_ERROR_SYSCALL,

        /** Could not read the object's ACL data.
         */
        ERR_COULD_NOT_READ_ACL,

        /** Authentication mechanism not allowed.
         */
        ERR_AUTH_MECH_NOT_ALLOWED,

        /** Run frame-stack level exceeded.
         */
        ERR_FRAMESTACK_LEVEL_EXCEEDED,

        /** An object of this class may not be placed under the specified parent.
         */
        ERR_SOFTRULES_DENIED_PLACEMENT,

        /** Regular expression failure.
         */
        ERR_REGEX_FAILURE,

        /** Server is being purged
         */
        ERR_SERVER_PURGING,

        /** Server has purged and is OK
         */
        ERR_SERVER_PURGED,

        /** Server must purge before embedding etc
         */
        ERR_SERVER_MUST_PURGE,

        /** The supplied image is not valid
         */
        ERR_INVALID_IMAGE,

        /** The supplied filename is invalid.
         */
        ERR_INVALID_FILE,

        /** Cannot register a space with home server offline.
         */
        ERR_NEW_SPACE_OFFLINE,

        /** Server not available.
         */
        ERR_SERVER_NOT_AVAILABLE,

        /** Parser error
         */
        ERR_PARSER_ERROR,

        /** The service is invalid (WIN32-1060)
         */
        ERR_NOT_SERVICE,

        /** Service not started (WIN32-1062)
         */
        ERR_SERVICE_NOT_ACTIVE_1062,

        /** The service process could not connect to the service controller (WIN32-1063)
         */
        ERR_FAILED_SERVICE_CONTROLLER_CONNECT_1063,

        /** No openFabric environment could be found.
         */
        ERR_NO_OF_ENVIRON,

        /** Cannot delete myself in my own destructor.
         */
        ERR_DELETE_IN_DESTRUCTOR,

        /** Invalid return code
         */
        ERR_INVAL_CODE,

        /** Invalid response
         */
        ERR_INVAL_RESPONSE,

        /** Invalid type at runtime
         */
        ERR_INVAL_TYPE,

        /** PRINTS argument count mismatch
         */
        ERR_PRINTS_ARG_COUNT,

        /** Space rule not found
         */
        ERR_SPACERULE_NOT_FOUND,

        /** Server is currently stopping
         */
        ERR_SERVER_TERMINATING,

        /** Class identity/name mismatch
         */
        ERR_CLASS_IDNAME_MISMATCH,

        /** System/CLM class mismatch. Server out of date.
         */
        ERR_SYSCLM_CLASS_MISMATCH,

        /** Registry server name/id mismatch
         */
        ERR_SERVER_NAMEID_MISMATCH,

        /** Invalid administrative function
         */
        ERR_INVALID_ADMIN_FUNC,

        /** Demo user has expired
         */
        ERR_DEMOUSER_EXPIRED,

        /** A new search engine is building         */
        ERR_BUILDING,

        /** No current user.
         */
        ERR_NO_CURRENT_USER,

        /** Local server version is not compatible with remote server.
         */
        ERR_INCOMPAT_SERVER,

        /** Object must be checked out to be modified.
         */
        ERR_NEED_LOCK,

        /** Another user has already checked out this object - try again later.
         */
        ERR_LOCK_HELD,

        /** You have already checked out this object.
         */
        ERR_LOCK_YOURS,

        /** Error parsing xpath.
         */
        ERR_XML_PARSE_XPATH,

        /** Cannot find specified WSDLPPItem
         */
        ERR_WSDL_CANNOT_FIND_WSDLPPITEM,

        /** Missing targetNamespace
         */
        ERR_WSDL_CANNOT_FIND_TARGET_NAMESPACE,

        /** The supplied buffer is too small
         */
        ERR_BUFFER_TOO_SMALL,

        /** The supplied nonce has expired
         */
        ERR_NONCE_EXPIRED,

        /** This server is not a dictionary server
         */
        ERR_NOT_DICTIONARY_SERVER,

        /** Home and dictionary server mismatch
         */
        ERR_HOME_DICTIONARY_MISMATCH,

        /** Invalid access function
         */
        ERR_INVAL_ACCESS_FUNCTION,

        /** Home server could not be contacted.
         */
        ERR_HOMESERVER_NOTAVAIL,

        /** Object name is empty
         */
        ERR_OBJECT_NAME_EMPTY,

        /** Invalid (or empty) attachment
         */
        ERR_INVAL_ATTACHMENT,

        /** SSL decryption error
         */
        ERR_SSL_DECRYPT,

        /** Too many server sites
         */
        ERR_TOO_MANY_SITES,

        /** Single sign on failed
         */
        ERR_SSO_FAILED
    };

/** \enum OFVALUE list types 
 */
enum OFVALUE_LIST_TYPES
    {
        /** integer list
         */
        typeListInteger = typeInteger|8,

        /** boolean
         */
        typeListBoolean = typeBoolean|8,

        /** string
         */
        typeListString = typeString|8,

        /** float
         */
        typeListFloat = typeFloat|8,

        /** datetime
         */
        typeListOFDateTime = typeOFDateTime|8,

        /** identity
         */
        typeListIdentity = typeIdentity|8,

        /** binary
         */
        typeListBinary = typeBinary|8
    };

/** \enum Operating systems */
enum OPSYS
    {
        /** Linux
         */
        OS_LINUX = 1,

        /** Windows 95/98
         */
        OS_WIN9X,

        /** Windows ME
         */
        OS_WINME,

        /** Windows NT
         */
        OS_WINNT,

        /** Windows 2000
         */
        OS_WIN2K,

        /** Windows XP
         */
        OS_WINXP,

        /** Mac OS
         */
        OS_MACOS,

        /** Mac OS X
         */
        OS_MACOSX,

        /** Solaris 8
         */
        OS_SOL8,

        /** Solaris 9
         */
        OS_SOL9,

        /** FreeBSD
         */
        OS_FREEBSD,

        /** Darwin
         */
        OS_DARWIN,

        /** Solaris
         */
        OS_SOLARIS
    };



/*
The following functions are generated for each enum:

Convert the supplied enumValue to the text name
A    void translate_ENUM( ofuint32 enumValue, char *enumName );

Same as above, but returns the enum text value
B    const char * translate_ENUM2( ofuint32 enumValue );

Converts a bitmask of enum values into a list containing their text values
Includes an optional trim of the value (removes the first X chars from the text value, 
where X is the number of characters in the enum name
C    void translate_ENUM_list( ofuint32 flags, char *listof, char trim = 0 );

Performs a reverse lookup, given the text value, returns the enum value.
D    ofuint32 translate_ENUM_rev( const char *enumName );

Same as above, but performs the lookup on the enum description.
E    ofuint32 translate_ENUM_rev_desc( const char *enumName );


F    void translate_ENUM_rev_list_desc( ofuint32 flags, char *listof );

Retrieves the enum description for a enum value
G    const char * translate_ENUM_desc( ofuint32 enumValue );

Checks if a supplied enum value is valid.
H    bool OFSYS_API isvalid_( ofuint32 enumValue );
Translates a bitmask (list) enum value to a description list.
I    void translate_ENUM_list_desc( ofuint32 enumValue, char *listof );


*/
/* LIST_OPERATIONS
*/


/* OFFILE_OPEN_FLAGS
*/


/* OFFILE_POSITION_FLAGS
*/


/* OFFILE_FILETYPES
*/


/* OFVALUE_TYPES
*/

OFSYS_API const char * translate_OFVALUE_TYPES2( ofuint32 enumValue );/*B*/
ofuint32 OFSYS_API translate_OFVALUE_TYPES_rev( const char *enumName );/*D*/
ofuint32 OFSYS_API translate_OFVALUE_TYPES_rev_desc( const char *enumName );/*E*/

/* OFERROR
*/

OFSYS_API const char * translate_OFERROR2( ofuint32 enumValue );/*B*/
OFSYS_API const char * translate_OFERROR_desc( ofuint32 enumValue );/*G*/

/* OFVALUE_LIST_TYPES
*/


/* OPSYS
*/

OFSYS_API const char * translate_OPSYS2( ofuint32 enumValue );/*B*/

