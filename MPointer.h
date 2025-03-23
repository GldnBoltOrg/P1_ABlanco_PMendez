//
// Created by Xpc on 23/3/2025.
//
#ifndef MPOINTER_H
#define MPOINTER_H

#include <iostream>
#include <grpcpp/grpcpp.h>
#include "memory_manager.grpc.pb.h"  // Archivo generado por gRPC

template <typename T>
class MPointer {
private:
    int id;  // ID del bloque de memoria en el servidor
    static std::shared_ptr<MemoryManager::Stub> stub;  // Cliente gRPC

public:
    // Inicializa la conexión con el servidor (debe llamarse al inicio)
    static void Init(const std::string& server_address) {
        stub = MemoryManager::NewStub(grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials()));
    }

    // Solicita un nuevo bloque de memoria
    static MPointer<T> New() {
        MPointer<T> ptr;
        grpc::ClientContext context;
        NewBlockRequest request;
        NewBlockResponse response;

        auto status = stub->NewBlock(&context, request, &response);
        if (status.ok()) {
            ptr.id = response.id();
        } else {
            throw std::runtime_error("Error al solicitar memoria remota");
        }
        return ptr;
    }

    // Sobrecarga de operador * para obtener y asignar valores
    T operator*() const {
        grpc::ClientContext context;
        GetValueRequest request;
        GetValueResponse response;

        request.set_id(id);
        auto status = stub->GetValue(&context, request, &response);
        if (!status.ok()) {
            throw std::runtime_error("Error al obtener valor remoto");
        }
        return response.value();
    }

    void operator*=(T value) {
        grpc::ClientContext context;
        SetValueRequest request;
        SetValueResponse response;

        request.set_id(id);
        request.set_value(value);
        auto status = stub->SetValue(&context, request, &response);
        if (!status.ok()) {
            throw std::runtime_error("Error al asignar valor remoto");
        }
    }

    // Sobrecarga de operador & para obtener el ID
    int operator&() const {
        return id;
    }

    // Sobrecarga de operador = para copiar referencia
    void operator=(const MPointer<T>& other) {
        this->id = other.id;
        grpc::ClientContext context;
        IncrementRefRequest request;
        IncrementRefResponse response;

        request.set_id(id);
        stub->IncrementRef(&context, request, &response);
    }

    // Destructor: libera la referencia en el servidor
    ~MPointer() {
        grpc::ClientContext context;
        FreeBlockRequest request;
        FreeBlockResponse response;

        request.set_id(id);
        stub->FreeBlock(&context, request, &response);
    }
};

// Definición del stub compartido
template <typename T>
std::shared_ptr<MemoryManager::Stub> MPointer<T>::stub = nullptr;

#endif
