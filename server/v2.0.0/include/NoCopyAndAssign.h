#ifndef NOCOPYANDASSIGN_H
#define NOCOPYANDASSIGN_H

#ifndef NOCOPYANDASSIGN
#define NOCOPYANDASSIGN(TYPENAME) TYPENAME(const TYPENAME &);\
	void operator=(const TYPENAME &)

#endif

class NoCopyAndAssign
{
public:
	//Constructor 
	/*
	 * Empty Constructor
	 */
	NoCopyAndAssign() {
	
	}
	//Destructor
	/*
	 * Empty Destructor
	 */
	virtual ~NoCopyAndAssign() {
		
	}
    /**
     * @return NoCopyAndAssign *
     */
    virtual NoCopyAndAssign * Clone () {
		return 0;
    }

private:
    /**
    * @param  rhs
    */
    void operator =(const NoCopyAndAssign &) {

    }
    /**
    * @param  rhs
    **/
	NoCopyAndAssign (const NoCopyAndAssign & ) {

    }

};

#endif // NOCOPYANDASSIGN_H
