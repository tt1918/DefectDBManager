//==============================================================================
//  (c) 2009 by Basler Vision Technologies
//==============================================================================
/**
\file  

\brief Errorcodes and defines for clBBProtocol-API

Errorcodes and defines for clbpp-API
*/
//==============================================================================
//==============================================================================
#pragma once

#ifndef CLBBPROTOCOLDEFS_H_INCLUDED_
#define CLBBPROTOCOLDEFS_H_INCLUDED_

// Basler specific errors

#define CL_ERR_BBP_MISSING_ACK              -30201      ///< Protocol error: Could not receive an ACK from camera.
#define CL_ERR_BBP_MISSING_BFS              -30202      ///< Protocol error: Missing BFS in frame.
#define CL_ERR_BBP_UNEXPECTED_OPCODE        -30203      ///< Protocol error: Unexpected opcode in frame.
#define CL_ERR_BBP_INVALID_DATALEN          -30204      ///< Protocol error: Invalid number of data bytes in frame.
#define CL_ERR_BBP_MISSING_BCC              -30205      ///< Protocol error: Received frame is missing BCC.
#define CL_ERR_BBP_INVALID_BCC              -30206      ///< Protocol error: Received frame has invalid BCC.
#define CL_ERR_BBP_MISSING_BFE              -30207      ///< Protocol error: Received frame has no BFE
#define CL_ERR_BBP_NAK                      -30208      ///< Protocol error: Camera returned NAK
#define CL_ERR_BBP_INVALID_PARAM            -30211      ///< Invalid param passed to function.
#define CL_ERR_BBP_INVALID_FRAME            -30212      ///< Protocol error: Received frame is not valid or BCC is invalid.
#define CL_ERR_BBP_NOT_FOUND                -30214      ///< Entry or item not found.

#endif // CLBBPROTOCOLDEFS_H_INCLUDED_
