
/** \file
 *	\ingroup Utilities
*/

namespace nxpl {

/**
 * Reference counting class.
 */
class RefCount
{
    private:
    int cnt; // Reference count

    public:
    RefCount() : cnt(0) {}

    void add()
    {
        // Increment the reference count
        cnt++;
    }
    int release()
    {
        // Decrement the reference count and
        // return the reference count.
        return --cnt;
    }
};

}
