#pragma once

#include "stdafx.h"
#include "CppUnitTest.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Rect.h"

#include "Texture.h"

using namespace Arcana;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<Vector2f>(const Vector2f& t) {
				std::string s = "Vector2f(" + std::to_string(t.x) + ", " + std::to_string(t.y) + ")";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Vector2i>(const Vector2i& t) {
				std::string s = "Vector2i(" + std::to_string(t.x) + ", " + std::to_string(t.y) + ")";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Vector2d>(const Vector2d& t) {
				std::string s = "Vector2d(" + std::to_string(t.x) + ", " + std::to_string(t.y) + ")";
				return std::wstring(s.begin(), s.end());
			}

			template<> static std::wstring ToString<Vector3f>(const Vector3f& t) {
				std::string s = "Vector3f(" + std::to_string(t.x) + ", " + std::to_string(t.y) + ", " + std::to_string(t.z) + ")";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Vector3i>(const Vector3i& t) {
				std::string s = "Vector3i(" + std::to_string(t.x) + ", " + std::to_string(t.y) + ", " + std::to_string(t.z) + ")";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Vector3d>(const Vector3d& t) {
				std::string s = "Vector3d(" + std::to_string(t.x) + ", " + std::to_string(t.y) + ", " + std::to_string(t.z) + ")";
				return std::wstring(s.begin(), s.end());
			}

			template<> static std::wstring ToString<Vector4f>(const Vector4f& t) {
				std::string s = "Vector4f(" + std::to_string(t.x) + ", " + std::to_string(t.y) + ", " + std::to_string(t.z) + ", " + std::to_string(t.w) + ")";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Vector4i>(const Vector4i& t) {
				std::string s = "Vector4i(" + std::to_string(t.x) + ", " + std::to_string(t.y) + ", " + std::to_string(t.z) + ", " + std::to_string(t.w) + ")";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Vector4d>(const Vector4d& t) {
				std::string s = "Vector4d(" + std::to_string(t.x) + ", " + std::to_string(t.y) + ", " + std::to_string(t.z) + ", " + std::to_string(t.w) + ")";
				return std::wstring(s.begin(), s.end());
			}

			template<> static std::wstring ToString<Rectf>(const Rectf& t) {
				std::string s = "Rectf(" + std::to_string(t.getLeft()) + ", " + std::to_string(t.getTop()) + ", " + std::to_string(t.getSize().x) + ", " + std::to_string(t.getSize().y) + ")";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Recti>(const Recti& t) {
				std::string s = "Recti(" + std::to_string(t.getLeft()) + ", " + std::to_string(t.getTop()) + ", " + std::to_string(t.getSize().x) + ", " + std::to_string(t.getSize().y) + ")";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Rectd>(const Rectd& t) {
				std::string s = "Rectd(" + std::to_string(t.getLeft()) + ", " + std::to_string(t.getTop()) + ", " + std::to_string(t.getSize().x) + ", " + std::to_string(t.getSize().y) + ")";
				return std::wstring(s.begin(), s.end());
			}

			template<> static std::wstring ToString<Matrix3f>(const Matrix3f& t) {
				std::string s = "Matrix3f";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Matrix3i>(const Matrix3i& t) {
				std::string s = "Matrix3i";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Matrix3d>(const Matrix3d& t) {
				std::string s = "Matrix3d";
				return std::wstring(s.begin(), s.end());
			}

			template<> static std::wstring ToString<Matrix4f>(const Matrix4f& t) {
				std::string s = "Matrix4f";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Matrix4i>(const Matrix4i& t) {
				std::string s = "Matrix4i";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Matrix4d>(const Matrix4d& t) {
				std::string s = "Matrix4d";
				return std::wstring(s.begin(), s.end());
			}

			template<> static std::wstring ToString<Texture::Format>(const Texture::Format& f) {
				std::string s = std::to_string(f);
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Texture::InternalFormat>(const Texture::InternalFormat& f) {
				std::string s = std::to_string(f);
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Texture::PixelType>(const Texture::PixelType& f) {
				std::string s = std::to_string(f);
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Texture::Type>(const Texture::Type& f) {
				std::string s = std::to_string(f);
				return std::wstring(s.begin(), s.end());
			}
		}
	}
}
