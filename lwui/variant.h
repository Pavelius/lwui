#include "crt.h"
#include "stringbuilder.h"

#pragma once

#define VKIND(T, V) template<> constexpr variant_s variant::kind<T>() { return V; }
#define VAR(T) bsmeta<T>::meta, bsdata<T>::source_ptr

struct bsreq;
enum variant_s : unsigned char;
union variant;

typedef sliceu<variant> variants;
typedef void (*fnscript)(int index, int bonus);
typedef void (*fngetinfo)(const void* object, variant v, stringbuilder& sb);

struct varianti {
	const char*			id;
	const bsreq*		metadata;
	array*				source;
	int                 key_count;
	fngetname			pgetname;
	fnstatus			pgetinfo;
	fngetinfo			pgetproperty;
	fnscript			pgscript;
	static const array* getarray(const void* object, const char* id);
	static const varianti* getsource(const char* id);
	static const varianti* getmetadata(const void* object);
	const char*			getid(const void* object) const;
	void				getinfo(const void* object, stringbuilder& sb) const;
	const char*			getname(const void* object) const;
	int					found(const char* id, size_t size) const;
	constexpr bool		isnamed() const { return key_count==1; }
	void				set(void* object, const char* id, void* value) const;
	void				set(void* object, const char* id, int value) const;
};
union variant {
	unsigned char		uc[4];
	unsigned			u;
	struct {
		unsigned short	value;
		char			counter;
		variant_s		type;
	};
	constexpr variant() : u(0) {}
	constexpr variant(int u) : u(u) {}
	template<class T> static constexpr variant_s kind();
	template<class T> variant(T* v) : variant((const void*)v) {}
	template<class T> constexpr variant(T v) : variant(kind<T>(), v) {}
	constexpr operator int() const { return u; }
	constexpr explicit operator bool() const { return u != 0; }
	constexpr bool operator==(const variant& v) const { return u == v.u; }
	constexpr bool operator!=(const variant& v) const { return u != v.u; }
	template<class T> operator T*() const { return (T*)((bsdata<varianti>::elements[type].source == bsdata<T>::source_ptr) ? getpointer() : 0); }
	void				clear() { u = 0; }
	constexpr bool		issame(const variant& v) const { return type == v.type && value == v.value; }
	template<class T> constexpr bool iskind() const { return bsdata<varianti>::elements[type].source==bsdata<T>::source_ptr; }
	const char*			getdescription() const;
	const varianti&		geti() const { return bsdata<varianti>::elements[type]; }
	const char*			getid() const;
	void*				getpointer() const { return geti().source->ptr(value); }
	const char*			getname() const;
	void				setvariant(variant_s t, unsigned short v) { type = t; value = v; counter = 0; }
};
template<> variant::variant(const char* v);
template<> variant::variant(const void* v);
