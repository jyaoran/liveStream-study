/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-10 23:15:44
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-10 23:21:42
 * @FilePath: /liveStream-study/src/base/Singleton.h
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#pragma once
#include "NonCopyable.h"
#include <pthread.h>

namespace liveStream
{
    namespace base
    {
        template <typename T>
        class Singleton : public NonCopyable
        {
        public:
            Singleton() = default;
            ~Singleton() = default;

            static T *&Instance();

        private:
            static void init();

            static pthread_once_t m_ponce;
            static T *m_pInstance;
        };

        template <typename T>
        pthread_once_t Singleton<T>::m_ponce = PTHREAD_ONCE_INIT;

        template <typename T>
        T *Singleton<T>::m_pInstance = nullptr;

        template <typename T>
        T *&Singleton<T>::Instance()
        {
            pthread_once(&m_ponce, &Singleton::init);
            return m_pInstance;
        }

        template <typename T>
        void Singleton<T>::init()
        {
            if (!m_pInstance)
            {
                m_pInstance = new T();
            }
        }
        
    } // namespace base
} // namespace liveStream