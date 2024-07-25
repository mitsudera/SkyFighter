#pragma once

#include "CoreMinimal.h"

//*****************************************************************************
//Buffer基底クラス
//*****************************************************************************

//*****************************************************************************
//Buffer基底クラス
//*****************************************************************************

#pragma once

#include "CoreMinimal.h"
template <typename Type>
class Buffer {
public:
    Buffer();
    Buffer(ID3D11Device* device);
    Buffer(ID3D11Device* device, int maxSize);

    ~Buffer();

    void Resize(ID3D11Device* device, int newSize);

    ID3D11Buffer* GetBuffer() const;

    void SetToBuffer(ID3D11DeviceContext* context, Type* data);
    void SetPS(ID3D11DeviceContext* context, UINT slot) const;
    void SetVS(ID3D11DeviceContext* context, UINT slot) const;
    void SetCS(ID3D11DeviceContext* context, UINT slot) const;

    void ReleaseBuffer();

private:
    void CreateBuffer(ID3D11Device* device, int size);

    ID3D11Buffer* m_buffer;
    int m_maxSize;
};

template <typename Type>
Buffer<Type>::Buffer() : m_buffer(nullptr), m_maxSize(0) {}

template <typename Type>
Buffer<Type>::Buffer(ID3D11Device* device) : m_maxSize(1), m_buffer(nullptr) {
    CreateBuffer(device, m_maxSize);
}

template <typename Type>
Buffer<Type>::Buffer(ID3D11Device* device, int maxSize) : m_maxSize(maxSize), m_buffer(nullptr) {
    CreateBuffer(device, m_maxSize);
}

template <typename Type>
Buffer<Type>::~Buffer() {
    ReleaseBuffer();
}

template <typename Type>
void Buffer<Type>::Resize(ID3D11Device* device, int newSize) {
    m_maxSize = newSize;
    CreateBuffer(device, newSize);
}

template <typename Type>
ID3D11Buffer* Buffer<Type>::GetBuffer() const {
    return m_buffer;
}

template <typename Type>
void Buffer<Type>::SetToBuffer(ID3D11DeviceContext* context, Type* data) {
    context->UpdateSubresource(m_buffer, 0, NULL, data, 0, 0);
}

template <typename Type>
void Buffer<Type>::SetPS(ID3D11DeviceContext* context, UINT slot) const {
    context->PSSetConstantBuffers(slot, 1, &m_buffer);
}

template <typename Type>
void Buffer<Type>::SetVS(ID3D11DeviceContext* context, UINT slot) const {
    context->VSSetConstantBuffers(slot, 1, &m_buffer);
}

template <typename Type>
void Buffer<Type>::SetCS(ID3D11DeviceContext* context, UINT slot) const {
    context->CSSetConstantBuffers(slot, 1, &m_buffer);
}

template <typename Type>
void Buffer<Type>::CreateBuffer(ID3D11Device* device, int size) {
    ReleaseBuffer();

    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.ByteWidth = sizeof(Type) * size;
    bufferDesc.StructureByteStride = sizeof(float);

    HRESULT hr = device->CreateBuffer(&bufferDesc, NULL, &m_buffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create buffer");
    }
}

template <typename Type>
void Buffer<Type>::ReleaseBuffer() {
    if (m_buffer) {
        m_buffer->Release();
        m_buffer = nullptr;
    }
}
