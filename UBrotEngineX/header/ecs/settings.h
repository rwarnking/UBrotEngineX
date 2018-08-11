#pragma once
///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: settings.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SETTINGS_H_
#define _SETTINGS_H_


#include <vector>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../../../mpl/Utils.hpp"


///////////////////////////////////////////////////////////////////////////////////////////////////
// Class name: None
/// TODO
///////////////////////////////////////////////////////////////////////////////////////////////////
//namespace ubrot
//{
namespace ecs
{
ECS_STRONG_TYPEDEF(std::size_t, DataIndex);
ECS_STRONG_TYPEDEF(std::size_t, EntityIndex);

// We're going to need two additional strong typedefs:
// * One for indices of handle data (auxiliary array indices).
// * One for the type of handle counter.

ECS_STRONG_TYPEDEF(std::size_t, HandleDataIndex);
ECS_STRONG_TYPEDEF(int, Counter);

namespace Impl
{
template <typename TSettings>
struct Entity
{
	using Settings = TSettings;
	using Bitset = typename Settings::Bitset;

	DataIndex dataIndex;

	// Entities must be aware of what handle is pointing
	// to them, so that the handle can be updated if the
	// entity gets moved around after a `refresh()`.
	HandleDataIndex handleDataIndex;

	Bitset bitset;
	bool alive;
};

// Our "handle data" class will be a simple pair
// of `EntityIndex` and `Counter`.
struct HandleData
{
	EntityIndex entityIndex;
	Counter counter;
};

// And our actual `Handle` class will be a simple pair
// of `HandleDataIndex` and `Counter`.
struct Handle
{
	HandleDataIndex handleDataIndex;
	Counter counter;
};

// To access an entity through an handle, these steps
// will occur:
//
// 1. The corresponding `HandleData` is retrieved,
//    using the `handleDataIndex` stored in the handle.
//
// 2. If the handle's counter does not match the handle
//    data's counter, then the handle is not valid anymore.
//
// 3. Otherwise, the entity will be retrieved through the
//    handle data's `entityIndex` member.
//

template <typename TSettings>
class ComponentStorage
{
private:
	using Settings = TSettings;
	using ComponentList = typename Settings::ComponentList;

	template <typename... Ts>
	using TupleOfVectors = std::tuple<std::vector<Ts>...>;

	MPL::Rename<TupleOfVectors, ComponentList> vectors;

public:
	void grow(std::size_t mNewCapacity)
	{
		Utils::forTuple(
			[this, mNewCapacity](auto& v)
		{
			v.resize(mNewCapacity);
		},
			vectors);
	}

	template <typename T>
	auto& getComponent(DataIndex mI) noexcept
	{
		return std::get<std::vector<T>>(vectors)[mI];
	}
};
}

// Our "list classes" will be simple wrappers around MPL type lists.

// Signatures will be compile-time lists of required components
// and tags. They will be listed in an MPL type list.
template <typename... Ts>
using Signature = MPL::TypeList<Ts...>;
template <typename... Ts>
using SignatureList = MPL::TypeList<Ts...>;

// Components and tags require no library-side code.
// They will be simple structs defined by the user, listed in MPL
// type lists.
template <typename... Ts>
using ComponentList = MPL::TypeList<Ts...>;
template <typename... Ts>
using TagList = MPL::TypeList<Ts...>;

// To actually check if an entity matches a specific signature
// at run-time, we'll make use of `std::bitset` bit operations.
// Every entity will have a bitset keeping track of its current
// components and tags.
// Every signature will have a bitset representing the required
// components and tags.

// Let's forward-declare a class that will match signatures
// to their respective bitsets, and also a class that will later
// be instantiated to actually store the signature bitsets.

namespace Impl
{
template <typename TSettings>
struct SignatureBitsets;

template <typename TSettings>
class SignatureBitsetsStorage;
}

// Let's now implement our `Settings` class.
// It will take the type lists defined by the user as template
// parameters.
// It will be easy to expand with additional compile-time settings
// if required in the future.

template <typename TComponentList, typename TTagList, typename TSignatureList>
struct Settings
{
	// Type aliases.
	using ComponentList = typename TComponentList::TypeList;
	using TagList = typename TTagList::TypeList;
	using SignatureList = typename TSignatureList::TypeList;
	using ThisType = Settings<ComponentList, TagList, SignatureList>;

	// "Inner" types aliases.
	using SignatureBitsets = Impl::SignatureBitsets<ThisType>;
	using SignatureBitsetsStorage = Impl::SignatureBitsetsStorage<ThisType>;

	// "Type traits".
	template <typename T>
	static constexpr bool isComponent() noexcept
	{
		return MPL::Contains<T, ComponentList>{};
	}
	template <typename T>
	static constexpr bool isTag() noexcept
	{
		return MPL::Contains<T, TagList>{};
	}
	template <typename T>
	static constexpr bool isSignature() noexcept
	{
		return MPL::Contains<T, SignatureList>{};
	}

	// Count of the various elements.
	static constexpr std::size_t componentCount() noexcept
	{
		return MPL::size<ComponentList>();
	}
	static constexpr std::size_t tagCount() noexcept
	{
		return MPL::size<TagList>();
	}
	static constexpr std::size_t signatureCount() noexcept
	{
		return MPL::size<SignatureList>();
	}

	// Unique IDs for the various elements.
	template <typename T>
	static constexpr std::size_t componentID() noexcept
	{
		return MPL::IndexOf<T, ComponentList>{};
	}
	template <typename T>
	static constexpr std::size_t tagID() noexcept
	{
		return MPL::IndexOf<T, TagList>{};
	}
	template <typename T>
	static constexpr std::size_t signatureID() noexcept
	{
		return MPL::IndexOf<T, SignatureList>{};
	}

	// NOTE: C++14 variable templates could have been used here,
	// but a bug present both in clang and gcc prevents it.
	// See: http://stackoverflow.com/questions/32050119

	// Bitsets for component types and tag types.
	using Bitset = std::bitset<componentCount() + Settings::tagCount()>;
	// TODO hier Settings:: warum? und warum nur bei einem?

	// Bit indices for components and tags.
	template <typename T>
	static constexpr std::size_t componentBit() noexcept
	{
		return componentID<T>();
	}
	template <typename T>
	static constexpr std::size_t tagBit() noexcept
	{
		// Tag bits will be stored towards the end of the bitset.
		return componentCount() + tagID<T>();
	}
};

namespace Impl
{
// Implementation of the `SignatureBitsets` class.
template <typename TSettings>
struct SignatureBitsets
{
	using Settings = TSettings;
	using ThisType = SignatureBitsets<Settings>;
	using SignatureList = typename Settings::SignatureList;
	using Bitset = typename Settings::Bitset;

	using BitsetStorage =
		MPL::Tuple<MPL::Repeat<Settings::signatureCount(), Bitset>>;

	template <typename T>
	using IsComponentFilter = std::integral_constant<bool,
		Settings::template isComponent<T>()>;

	template <typename T>
	using IsTagFilter =
		std::integral_constant<bool, Settings::template isTag<T>()>;

	template <typename TSignature>
	using SignatureComponents =
		MPL::Filter<IsComponentFilter, TSignature>;

	template <typename TSignature>
	using SignatureTags = MPL::Filter<IsTagFilter, TSignature>;
};

// Let's now implement the class that will actually store
// the bitsets. This will be instantiated in our `Manager` later.
template <typename TSettings>
class SignatureBitsetsStorage
{
private:
	using Settings = TSettings;
	using SignatureBitsets = typename Settings::SignatureBitsets;
	using SignatureList = typename SignatureBitsets::SignatureList;
	using BitsetStorage = typename SignatureBitsets::BitsetStorage;

	BitsetStorage storage;

public:
	template <typename T>
	auto& getSignatureBitset() noexcept
	{
		static_assert(Settings::template isSignature<T>(), "");

		// As previously mentioned, we can easily get the bitset
		// by using `std::get` with the unique ID of the signature.
		return std::get<Settings::template signatureID<T>()>(storage);
	}

	template <typename T>
	const auto& getSignatureBitset() const noexcept
	{
		static_assert(Settings::template isSignature<T>(), "");
		return std::get<Settings::template signatureID<T>()>(storage);
	}

private:
	// We'll have to initialize the bitsets at run-time. This
	// function will take care of initializing the bitset for a
	// single signature `T`.
	template <typename T>
	void initializeBitset() noexcept
	{
		auto& b(this->getSignatureBitset<T>());

		// Compile-time list of required components.
		using SignatureComponents =
			typename SignatureBitsets::template SignatureComponents<T>;

		// Compile-time list of required tags.
		using SignatureTags =
			typename SignatureBitsets::template SignatureTags<T>;

		MPL::forTypes<SignatureComponents>([this, &b](auto t)
		{
			// The `ECS_TYPE(t)` macro expands to:
			// `typename decltype(t)::type`.

			b[Settings::template componentBit<ECS_TYPE(t)>()] =
				true;
		});

		MPL::forTypes<SignatureTags>([this, &b](auto t)
		{
			b[Settings::template tagBit<ECS_TYPE(t)>()] = true;
		});
	}

public:
	// Lastly, let's initialize all bitsets on construction.
	SignatureBitsetsStorage() noexcept
	{
		MPL::forTypes<SignatureList>([this](auto t)
		{
			this->initializeBitset<ECS_TYPE(t)>();
		});
	}
};
};
};
#endif // !_SETTINGS_H_