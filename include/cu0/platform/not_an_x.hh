#ifndef CU0_NOT_AN_X_HH__
#define CU0_NOT_AN_X_HH__

/*!
 * defines not_an_x if this platform was detected as not an x platform
 *     @example linux @example unix
 */
#ifndef __unix__
#ifdef not_an_x
#warning not_an_x has been aready defined somewhere else => not redefining it
#else
#define not_an_x
#endif
#endif

#endif /// CU0_NOT_AN_X_HH__
