///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: manager.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MANAGER_H_
#define _MANAGER_H_


//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "defs.h"


//namespace ubrot
//{
namespace ecs
{
///////////////////////////////////////////////////////////////////////////////////////////////////
// Class name: Manager
/// TODO
///////////////////////////////////////////////////////////////////////////////////////////////////
template <typename TSettings>
class Manager
{
private:
	using Settings = TSettings;
	using ThisType = Manager<Settings>;
	using Bitset = typename Settings::Bitset;
	using Entity = Impl::Entity<Settings>;
	using HandleData = Impl::HandleData;

public:
	using Handle = Impl::Handle;

private:
	using SignatureBitsetsStorage = Impl::SignatureBitsetsStorage<Settings>;
	using ComponentStorage = Impl::ComponentStorage<Settings>;

	std::size_t capacity{ 0 }, size{ 0 }, sizeNext{ 0 };
	std::vector<Entity> entities;
	SignatureBitsetsStorage signatureBitsets;
	ComponentStorage components;

	// Handle data will be stored in a vector, like entities.
	std::vector<HandleData> handleData;

	void growTo(std::size_t mNewCapacity)
	{
		assert(mNewCapacity > capacity);

		entities.resize(mNewCapacity);
		components.grow(mNewCapacity);

		// Do not forget to grow the new container.
		handleData.resize(mNewCapacity);

		for (auto i(capacity); i < mNewCapacity; ++i)
		{
			auto& e(entities[i]);
			auto& h(handleData[i]);

			e.dataIndex = i;
			e.bitset.reset();
			e.alive = false;

			// New entities will need to know what their
			// handle is. During initialization, it will
			// be the handle "directly below them".
			e.handleDataIndex = i;

			// New handle data instances will have to
			// be initialized with a value for their counter
			// and the index of the entity they're pointing
			// at (which will be the one "directly on top of
			// them", at that point in time).

			// Set handledata values.
			h.counter = 0;
			h.entityIndex = i;
		}

		capacity = mNewCapacity;
	}

	void growIfNeeded()
	{
		if (capacity > sizeNext) return;
		growTo((capacity + 10) * 2);
	}

	auto& getEntity(EntityIndex mI) noexcept
	{
		assert(sizeNext > mI);
		return entities[mI];
	}

	const auto& getEntity(EntityIndex mI) const noexcept
	{
		assert(sizeNext > mI);
		return entities[mI];
	}

	// We'll need some getters for `handleData`.
	// We can get `HandleData` instances in three ways:
	// * Through an `HandleDataIndex`.
	// * Through an `EntityIndex` -> `HandleDataIndex`.
	// * Through an `Handle` -> `HandleDataIndex`.

	// Some code repetition is necessary...

	auto& getHandleData(HandleDataIndex mI) noexcept
	{
		// The handle for an entity does not necessarily have to
		// be before the `sizeNext` index.
		assert(handleData.size() > mI);
		return handleData[mI];
	}
	const auto& getHandleData(HandleDataIndex mI) const noexcept
	{
		assert(handleData.size() > mI);
		return handleData[mI];
	}

	auto& getHandleData(EntityIndex mI) noexcept
	{
		return getHandleData(getEntity(mI).handleDataIndex);
	}
	const auto& getHandleData(EntityIndex mI) const noexcept
	{
		return getHandleData(getEntity(mI).handleDataIndex);
	}

	auto& getHandleData(const Handle& mX) noexcept
	{
		return getHandleData(mX.handleDataIndex);
	}
	const auto& getHandleData(const Handle& mX) const noexcept
	{
		return getHandleData(mX.handleDataIndex);
	}

public:
	Manager() {
		growTo(100);
	}

	// How to check if an handle is valid?
	// Comparing its counter to the corresponding handle data
	// counter is enough.
	auto isHandleValid(const Handle& mX) const noexcept
	{
		return mX.counter == getHandleData(mX).counter;
	}

	// All methods that we previously could call with `EntityIndex`
	// should also be possible to call using handles.

	// Let's create a method that returns an `EntityIndex` from
	// an handle valid to aid us.

	auto getEntityIndex(const Handle& mX) const noexcept
	{
		assert(isHandleValid(mX));
		return getHandleData(mX).entityIndex;
	}

	auto isAlive(EntityIndex mI) const noexcept
	{
		return getEntity(mI).alive;
	}
	auto isAlive(const Handle& mX) const noexcept
	{
		return isAlive(getEntityIndex(mX));
	}

	void kill(EntityIndex mI) noexcept {
		getEntity(mI).alive = false;
	}
	void kill(const Handle& mX) noexcept {
		kill(getEntityIndex(mX));
	}

	template <typename T>
	auto hasTag(EntityIndex mI) const noexcept
	{
		static_assert(Settings::template isTag<T>(), "");
		return getEntity(mI).bitset[Settings::template tagBit<T>()];
	}
	template <typename T>
	auto hasTag(const Handle& mX) const noexcept
	{
		return hasTag<T>(getEntityIndex(mX));
	}

	template <typename T>
	void addTag(EntityIndex mI) noexcept
	{
		static_assert(Settings::template isTag<T>(), "");
		getEntity(mI).bitset[Settings::template tagBit<T>()] = true;
	}
	template <typename T>
	void addTag(const Handle& mX) noexcept
	{
		return addTag<T>(getEntityIndex(mX));
	}

	template <typename T>
	void delTag(EntityIndex mI) noexcept
	{
		static_assert(Settings::template isTag<T>(), "");
		getEntity(mI).bitset[Settings::template tagBit<T>()] = false;
	}
	template <typename T>
	auto delTag(const Handle& mX) noexcept
	{
		return delTag<T>(getEntityIndex(mX));
	}

	template <typename T>
	auto hasComponent(EntityIndex mI) const noexcept
	{
		static_assert(Settings::template isComponent<T>(), "");
		return getEntity(mI).bitset[Settings::template componentBit<T>()];
	}
	template <typename T>
	auto hasComponent(const Handle& mX) const noexcept
	{
		return hasComponent<T>(getEntityIndex(mX));
	}

	template <typename T, typename... TArgs>
	auto& addComponent(EntityIndex mI, TArgs&&... mXs) noexcept
	{
		static_assert(Settings::template isComponent<T>(), "");

		auto& e(getEntity(mI));
		e.bitset[Settings::template componentBit<T>()] = true;

		auto& c(components.template getComponent<T>(e.dataIndex));
		//c = T(FWD(mXs)...); ?
		c = T(ECS_FWD(mXs)...);
		return c;
	}
	template <typename T, typename... TArgs>
	auto& addComponent(const Handle& mX, TArgs&&... mXs) noexcept
	{
		return addComponent<T>(getEntityIndex(mX), ECS_FWD(mXs)...); // FWD ?
	}

	// `getComponent` will simply return a reference to the
	// component, after asserting its existance.
	template <typename T>
	auto& getComponent(EntityIndex mI) noexcept
	{
		static_assert(Settings::template isComponent<T>(), "");
		assert(hasComponent<T>(mI));

		return components.template getComponent<T>(getEntity(mI).dataIndex);
	}
	template <typename T>
	auto& getComponent(const Handle& mX) noexcept
	{
		return getComponent<T>(getEntityIndex(mX));
	}

	template <typename T>
	void delComponent(EntityIndex mI) noexcept
	{
		static_assert(Settings::template isComponent<T>(), "");
		getEntity(mI).bitset[Settings::template componentBit<T>()] = false;
	}
	template <typename T>
	void delComponent(const Handle& mX) noexcept
	{
		return delComponent<T>(getEntityIndex(mX));
	}

	// If the user does not need to track a specific entity,
	// `createIndex()` can be used.
	auto createIndex()
	{
		growIfNeeded();
		EntityIndex freeIndex(sizeNext++);

		assert(!isAlive(freeIndex));
		auto& e(entities[freeIndex]);
		e.alive = true;
		e.bitset.reset();

		return freeIndex;
	}

	// Otherwise, we'll need to create a new method that
	// returns an handle.
	auto createHandle()
	{
		// Let's start by creating an entity with
		// `createIndex()`, and storing the result.
		auto freeIndex(createIndex());
		assert(isAlive(freeIndex));

		// We'll need to "match" the new entity
		// and the new handle together.
		auto& e(entities[freeIndex]);
		auto& hd(handleData[e.handleDataIndex]);

		// Let's update the entity's corresponding
		// handle data to point to the new index.
		hd.entityIndex = freeIndex;

		// Initialize a valid entity handle.
		Handle h;

		// The handle will point to the entity's
		// handle data...
		h.handleDataIndex = e.handleDataIndex;

		// ...and its validity counter will be set
		// to the handle data's current counter.
		h.counter = hd.counter;

		// Assert entity handle validity.
		assert(isHandleValid(h));

		// Return a copy of the entity handle.
		return h;
	}

	void clear() noexcept
	{
		// Let's re-initialize handles during `clear()`.

		for (auto i(0u); i < capacity; ++i)
		{
			auto& e(entities[i]);
			auto& hd(handleData[i]);

			e.dataIndex = i;
			e.bitset.reset();
			e.alive = false;
			e.handleDataIndex = i;

			hd.counter = 0;
			hd.entityIndex = i;
		}

		size = sizeNext = 0;
	}

	void refresh() noexcept
	{
		if (sizeNext == 0)
		{
			size = 0;
			return;
		}

		size = sizeNext = refreshImpl();
	}

	template <typename T>
	auto matchesSignature(EntityIndex mI) const noexcept
	{
		static_assert(Settings::template isSignature<T>(), "");

		const auto& entityBitset(getEntity(mI).bitset);
		const auto& signatureBitset(
			signatureBitsets.template getSignatureBitset<T>());

		return (signatureBitset & entityBitset) == signatureBitset;
	}

	template <typename TF>
	void forEntities(TF&& mFunction)
	{
		for (EntityIndex i{ 0 }; i < size; ++i) mFunction(i);
	}

	template <typename T, typename TF>
	void forEntitiesMatching(TF&& mFunction)
	{
		static_assert(Settings::template isSignature<T>(), "");

		forEntities([this, &mFunction](auto i)
		{
			if (matchesSignature<T>(i))
				expandSignatureCall<T>(i, mFunction);
		});
	}

private:
	template <typename... Ts>
	struct ExpandCallHelper;

	template <typename T, typename TF>
	void expandSignatureCall(EntityIndex mI, TF&& mFunction)
	{
		static_assert(Settings::template isSignature<T>(), "");

		using RequiredComponents = typename Settings::SignatureBitsets::
			template SignatureComponents<T>;

		using Helper = MPL::Rename<ExpandCallHelper, RequiredComponents>;

		Helper::call(mI, *this, mFunction);
	}

	template <typename... Ts>
	struct ExpandCallHelper
	{
		template <typename TF>
		static void call(EntityIndex mI, ThisType& mMgr, TF&& mFunction)
		{
			auto di(mMgr.getEntity(mI).dataIndex);

			mFunction(mI, mMgr.components.template getComponent<Ts>(di)...);
		}
	};

private:
	// We'll need to do something when iterating over dead
	// entities during `refreshImpl()` to make sure their
	// handles get invalidated.

	// Invalidating an handle is as simple as incrementing
	// its counter.
	void invalidateHandle(EntityIndex mX) noexcept
	{
		auto& hd(handleData[entities[mX].handleDataIndex]);
		++hd.counter;
	}

	// We'll also need that swapped alive entities' handles
	// point to the new correct indices.

	// It is sufficient to get the handle data from the entity,
	// after it has been swapped, and update its entity index
	// with the new one.
	void refreshHandle(EntityIndex mX) noexcept
	{
		auto& hd(handleData[entities[mX].handleDataIndex]);
		hd.entityIndex = mX;
	}

	auto refreshImpl() noexcept
	{
		EntityIndex iD{ 0 }, iA{ sizeNext - 1 };

		while (true)
		{
			for (; true; ++iD)
			{
				if (iD > iA) return iD;
				if (!entities[iD].alive) break;

				// There is no need to invalidate or refresh
				// handles of untouched alive entities.
			}

			for (; true; --iA)
			{
				if (entities[iA].alive) break;

				// New dead entities on the right need to be
				// invalidated. Their handle index doesn't need
				// to be changed.
				invalidateHandle(iA);

				if (iA <= iD) return iD;
			}

			assert(entities[iA].alive);
			assert(!entities[iD].alive);

			std::swap(entities[iA], entities[iD]);

			// After swap, the alive entity's handle must be
			// refreshed, but not invalidated.
			refreshHandle(iD);

			// After swap, the dead entity's handle must be
			// both refreshed and invalidated.
			invalidateHandle(iA);
			refreshHandle(iA);

			++iD;
			--iA;
		}

		return iD;
	}

public:
	auto getEntityCount() const noexcept {
		return size;
	}

	auto getCapacity() const noexcept {
		return capacity;
	}

	auto printState(std::ostream& mOSS) const
	{
		mOSS << "\nsize: " << size << "\nsizeNext: " << sizeNext
			<< "\ncapacity: " << capacity << "\n";

		for (auto i(0u); i < sizeNext; ++i)
		{
			auto& e(entities[i]);
			mOSS << (e.alive ? "A" : "D");
		}

		mOSS << "\n\n";
	}
};
};
//};

#endif // !_MANAGER_H_
