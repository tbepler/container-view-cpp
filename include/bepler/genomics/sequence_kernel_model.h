#ifndef INCLUDED_BEPLER_GENOMICS_SEQUENCE_KERNEL_MODEL_H
#define INCLUDED_BEPLER_GENOMICS_SEQUENCE_KERNEL_MODEL_H

#include "bepler/genomics/motif.h"
#include <memory>
#include <iostream>

namespace genomics{

    class SequenceKernelModel : public Motif{

        struct ModelImpl;
        std::unique_ptr< ModelImpl > impl_;

        public:
            virtual inline double score( const char* str ) const override;
            virtual inline double scoreAll( const char* begin, const char* end, functional::acceptor_f<double>&& out ) const override;
            friend bool operator==( const SequenceKernelModel& a, const SequenceKernelModel& b );
            friend bool operator!=( const SequenceKernelModel& a, const SequenceKernelModel& b );
            friend std::ostream& operator<<( std::ostream& out, const SequenceKernelModel& model );
            friend std::istream& operator>>( std::istream& in, SequenceKernelModel& model );
            

    }; //class SequenceKernelModel

} //namespace genomics

#endif
