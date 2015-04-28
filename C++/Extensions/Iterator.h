
namespace std {
	namespace Iterator {
		template <typename Iterator>
		Iterator next(Iterator it) {
			return ++it;
		}
	}
}