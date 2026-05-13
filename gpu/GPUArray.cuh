//
// Created by dominik on 31.03.26.
//

#ifndef GPURAYTRACER_ARRAY_H
#define GPURAYTRACER_ARRAY_H

#include <stdexcept>

#include "CUDAAnnotations.cuh"
#include <vector>

template<typename T>
struct ReadOnlyGPUArrayView
{
   public:
      GPU_HD ReadOnlyGPUArrayView() = delete;

      GPU_HD ReadOnlyGPUArrayView( T* data, size_t size );

      GPU_HD [[nodiscard]] size_t size() const;

      GPU_HD const T& operator[]( size_t index ) const;

      HOST_DEV const T& at( size_t index ) const;

      GPU_HD [[nodiscard]] bool inBounds( size_t index ) const;

      GPU_HD const T* begin() const;

      GPU_HD const T* end() const;

   protected:
      T* data = nullptr;
      size_t _size = 0;
};

template<typename T>
struct GPUArrayView : ReadOnlyGPUArrayView<T>
{
   GPU_HD GPUArrayView() = delete;

   GPU_HD GPUArrayView( T* data, size_t size );

   GPU_HD T& operator[]( size_t index );

   HOST_DEV T& at( size_t index );

   GPU_HD T* begin();

   GPU_HD T* end();
};

template<typename T>
struct GPUArray
{
   public:
      GPUArray() = delete;

      explicit GPUArray( size_t size );

      GPUArray( const GPUArray& ) = delete;

      GPUArray& operator=( const GPUArray& ) = delete;

      GPUArray( GPUArray&& other ) noexcept;

      ~GPUArray();

      void upload( const T* hostData, size_t size );

      void upload( const std::vector<T>& hostData );

      void download( T* hostData, size_t size ) const;

      [[nodiscard]] size_t size() const;

      GPUArrayView<T> view() const;

      ReadOnlyGPUArrayView<T> readOnlyView() const;

   private:
      T* data = nullptr;
      size_t _size = 0;

      void allocate( size_t size );

      void free() const;
};

template<typename T>
ReadOnlyGPUArrayView<T>::ReadOnlyGPUArrayView( T* data, size_t size ) : data( data ), _size( size )
{
}

template<typename T>
size_t ReadOnlyGPUArrayView<T>::size() const
{
   return _size;
}

template<typename T>
const T& ReadOnlyGPUArrayView<T>::operator[]( size_t index ) const
{
   return data[ index ];
}

template<typename T>
const T& ReadOnlyGPUArrayView<T>::at( size_t index ) const
{
   if( !inBounds( index ) )
      throw std::out_of_range( "GPUArrayView::at: index out of bounds" );

   return data[ index ];
}

template<typename T>
bool ReadOnlyGPUArrayView<T>::inBounds( size_t index ) const
{
   return index < _size;
}

template<typename T>
const T* ReadOnlyGPUArrayView<T>::begin() const
{
   return data;
}

template<typename T>
const T* ReadOnlyGPUArrayView<T>::end() const
{
   return data + _size;
}

template<typename T>
GPUArrayView<T>::GPUArrayView( T* data, size_t size ) : ReadOnlyGPUArrayView<T>( data, size )
{
}

template<typename T>
T& GPUArrayView<T>::operator[]( size_t index )
{
   return this->data[ index ];
}

template<typename T>
T& GPUArrayView<T>::at( size_t index )
{
   if( !this->inBounds( index ) )
      throw std::out_of_range( "GPUArrayView::at: index out of bounds" );

   return this->data[ index ];
}

template<typename T>
T* GPUArrayView<T>::begin()
{
   return this->data;
}

template<typename T>
T* GPUArrayView<T>::end()
{
   return this->data + this->_size;
}

template<typename T>
GPUArray<T>::GPUArray( size_t size )
{
   allocate( size );
   _size = size;
}

template<typename T>
GPUArray<T>::GPUArray( GPUArray&& other ) noexcept : data( other.data ), _size( other._size )
{
   other.data = nullptr;
   other._size = 0;
}

template<typename T>
GPUArray<T>::~GPUArray()
{
   free();
}

// Optionally add return value checks

template<typename T>
void GPUArray<T>::upload( const T* hostData, size_t size )
{
   if( size > _size )
      throw std::runtime_error( "GPUArray upload: upload size exceeds allocated size of the array" );

   cudaMemcpy( this->data, hostData, size * sizeof( T ), cudaMemcpyHostToDevice );
}

template<typename T>
void GPUArray<T>::upload( const std::vector<T>& hostData )
{
   upload( hostData.data(), hostData.size() );
}

template<typename T>
void GPUArray<T>::download( T* hostData, size_t size ) const
{
   if( size > _size )
      throw std::runtime_error( "GPUArray download: download size exceeds allocated size of the array" );

   cudaMemcpy( hostData, this->data, size * sizeof( T ), cudaMemcpyDeviceToHost );
}

template<typename T>
size_t GPUArray<T>::size() const
{
   return _size;
}

template<typename T>
GPUArrayView<T> GPUArray<T>::view() const
{
   return GPUArrayView<T>( data, _size );
}

template<typename T>
ReadOnlyGPUArrayView<T> GPUArray<T>::readOnlyView() const
{
   return ReadOnlyGPUArrayView<T>( data, _size );
}

template<typename T>
void GPUArray<T>::allocate( size_t size )
{
   cudaMalloc( reinterpret_cast<void**>( &data ), size * sizeof( T ) );
}

template<typename T>
void GPUArray<T>::free() const
{
   cudaFree( data );
}

#endif //GPURAYTRACER_ARRAY_H
