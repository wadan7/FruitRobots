#pragma once
#include <string>

namespace FSR {

	class Entity
	{
	public:
		Entity(const std::string& name)
			: m_Name(name)
		{
		}

		virtual ~Entity() {};

		inline virtual unsigned char GetSymbol() const noexcept { return '?'; }
		inline const std::string& GetName() const noexcept { return m_Name; }
		

	private:
		std::string m_Name;
		
	};

}