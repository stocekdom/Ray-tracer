//
// Created by dominik on 31.03.26.
//

#ifndef GPURAYTRACER_ARRAY_H
#define GPURAYTRACER_ARRAY_H

#include <stdexcept>

#include "CUDAAnnotations.h"
#include <vector>

template<typename T>
struct ReadOnlyGPUArrayView
{
   public:
      GPU_HD ReadOnlyGPUArrayView() = delete;

      GPU_HD ReadOnlyGPUArrayView( T* data, unsigned int size );

      GPU_HD [[nodiscard]] unsigned int size() const;

      GPU_HD const T& operator[]( unsigned int index ) const;

      HOST_DEV const T& at( unsigned int index ) const;

      GPU_HD [[nodiscard]] bool inBounds( unsigned int index ) const;

   protected:
      T* data = nullptr;
      unsigned int _size = 0;
};

template<typename T>
struct GPUArrayView : ReadOnlyGPUArrayView<T>
{
   GPU_HD GPUArrayView() = delete;

   GPU_HD GPUArrayView( T* data, unsigned int size );

   GPU_HD T& operator[]( unsigned int index );

   HOST_DEV T& at( unsigned int index );
};

template<typename T>
struct GPUArray
{
   public:
      GPUArray() = delete;

      GPUArray( const GPUArray& ) = delete;

      GPUArray& operator=( const GPUArray& ) = delete;

      explicit GPUArray( unsigned int size );

      GPUArray( GPUArray&& other ) noexcept;

      ~GPUArray();

      void upload( const T* hostData, unsigned int size );

      void upload( const std::vector<T>& hostData );

      void download( T* hostData, unsigned int size ) const;

      [[nodiscard]] unsigned int size() const;

      GPUArrayView<T> view() const;

      ReadOnlyGPUArrayView<T> readOnlyView() const;

   private:
      T* data = nullptr;
      unsigned int _size = 0;

      void allocate( unsigned int size );

      void free() const;
};

template<typename T>
ReadOnlyGPUArrayView<T>::ReadOnlyGPUArrayView( T* data, unsigned int size ) : data( data ), _size( size )
{
}

template<typename T>
unsigned int ReadOnlyGPUArrayView<T>::size() const
{
   return _size;
}

template<typename T>
const T& ReadOnlyGPUArrayView<T>::operator[]( unsigned int index ) const
{
   return data[ index ];
}

template<typename T>
const T& ReadOnlyGPUArrayView<T>::at( unsigned int index ) const
{
   if( !inBounds( index ) )
      throw std::out_of_range( "GPUArrayView::at: index out of bounds" );

   return data[ index ];
}

template<typename T>
bool ReadOnlyGPUArrayView<T>::inBounds( unsigned int index ) const
{
   return index < _size;
}

template<typename T>
GPUArrayView<T>::GPUArrayView( T* data, unsigned int size ) : ReadOnlyGPUArrayView<T>( data, size )
{
}

template<typename T>
T& GPUArrayView<T>::operator[]( unsigned int index )
{
   return this->data[ index ];
}

template<typename T>
T& GPUArrayView<T>::at( unsigned int index )
{
   if( !this->inBounds( index ) )
      throw std::out_of_range( "GPUArrayView::at: index out of bounds" );

   return this->data[ index ];
}

template<typename T>
GPUArray<T>::GPUArray( unsigned int size )
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
   free( data );
}

// Optionally add return value checks

template<typename T>
void GPUArray<T>::upload( const T* hostData, unsigned int size )
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
void GPUArray<T>::download( T* hostData, unsigned int size ) const
{
   if( size > _size )
      throw std::runtime_error( "GPUArray download: download size exceeds allocated size of the array" );

   cudaMemcpy( hostData, this->data, size * sizeof( T ), cudaMemcpyDeviceToHost );
}

template<typename T>
unsigned int GPUArray<T>::size() const
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
void GPUArray<T>::allocate( unsigned int size )
{
   cudaMalloc( static_cast<void**>( &data ), size * sizeof( T ) );
}

template<typename T>
void GPUArray<T>::free() const
{
   cudaFree( data );
}

#endif //GPURAYTRACER_ARRAY_H
