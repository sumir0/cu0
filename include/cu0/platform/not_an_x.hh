#ifndef CU0_NOT_AN_X_HH__
#define CU0_NOT_AN_X_HH__

/*!
 * defines NOT_AN_X if this platform was detected as not an x platform
 *     @example linux @example unix
 */
#if !defined(__unix__)
#if defined(NOT_AN_X)
#warning NOT_AN_X has been aready defined somewhere else => not redefining it
#else
#define NOT_AN_X
#endif
#endif

#endif /// CU0_NOT_AN_X_HH__
