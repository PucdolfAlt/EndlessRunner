/**
 * @file AuthModule.ixx
 * @brief Modu� odpowiedzialny za uwierzytelnianie u�ytkownika.
 *
 * Zawiera klas� Authorization oraz enumeracje do obs�ugi rejestracji i logowania.
 */


module;
#include <string>
#include <set>
#include <regex>
#include <map>

export module AuthModule;

/**
 * @enum SignInResult
 * @brief Wyniki operacji rejestracji u�ytkownika.
 */
export enum class SignInResult {
	SUCCESS,           /**< Rejestracja zako�czona sukcesem. */
	USERNAME_TAKEN,    /**< Nazwa u�ytkownika ju� istnieje. */
	INVALID_USERNAME,  /**< Nieprawid�owa nazwa u�ytkownika. */
	INVALID_PASSWORD   /**< Nieprawid�owe has�o. */
};


/**
 * @enum LogInResult
 * @brief Wyniki operacji logowania u�ytkownika.
 */
export enum class LogInResult {
	SUCCESS,           /**< Logowanie zako�czone sukcesem. */
	USER_NOT_FOUND,    /**< U�ytkownik nie znaleziony. */
	INCORRECT_PASSWORD /**< Nieprawid�owe has�o. */
};


/**
 * @class Authorization
 * @brief Klasa zarz�dzaj�ca uwierzytelnianiem u�ytkownika.
 *
 * Obs�uguje rejestracj� i logowanie, zapisuj�c dane u�ytkownik�w do pliku.
 */
export class Authorization {
private:
	/** @brief Zbi�r istniej�cych nazw u�ytkownik�w. */
	std::set<std::string> existingUsernames;
	/** @brief Mapa przechowuj�ca hashe hase� dla u�ytkownik�w. */
	std::map<std::string, std::string> storedPasswords;
	/** @brief Nazwa pliku przechowuj�cego dane u�ytkownik�w. */
	const std::string usersFile = "users.txt";

	/**
	 * @brief Hashuje has�o u�ytkownika.
	 * @param password Has�o do zahashowania.
	 * @return Zahashowane has�o jako ci�g znak�w.
	 * @note U�ywa std::hash; w produkcji nale�y u�y� bezpiecznego algorytmu.
	 */
	std::string hashPassword(const std::string& password);/* {
		std::hash<std::string> hasher;
		return std::to_string(hasher(password));
	}*/

	/**
	 * @brief Wczytuje dane u�ytkownik�w z pliku.
	 */
	void loadUsers(); /*{
		std::ifstream file(usersFile);
		if (!file.is_open()) {
			return;
		}

		std::string line;
		while (std::getline(file, line)) {
			std::istringstream iss(line);
			std::string username, hashedPassword;
			if (std::getline(iss, username, ':') && std::getline(iss, hashedPassword)) {
				existingUsernames.insert(username);
				storedPasswords[username] = hashedPassword;
			}
		}
		file.close();
	}*/

	/**
	 * @brief Zapisuje nowego u�ytkownika do pliku.
	 * @param username Nazwa u�ytkownika.
	 * @param hashedPassword Zahashowane has�o.
	 * @return True, je�li zapis si� powi�d�.
	 */
	bool saveUser(const std::string& username, const std::string& hashedPassword);//{
	//	std::ofstream file(usersFile, std::ios::app); // Append mode
	//	if (!file.is_open()) {
	//		return false;
	//	}
	//	file << username << ":" << hashedPassword << "\n";
	//	file.close();
	//	return true;
	//}

public:
	/**
	* @brief Konstruktor klasy Authorization.
	* @note Wczytuje istniej�cych u�ytkownik�w przy inicjalizacji.
	*/
	Authorization() {
		loadUsers();
	}

	/**
	 * @brief Rejestruje nowego u�ytkownika.
	 * @param username Nazwa u�ytkownika (3-20 znak�w alfanumerycznych).
	 * @param password Has�o (min. 8 znak�w, litery i cyfry).
	 * @return Wynik operacji rejestracji.
	 */
	SignInResult signIn(const std::string& username, const std::string& password); /*{
		if (std::ranges::find(existingUsernames, username) != existingUsernames.end()) {
			return SignInResult::USERNAME_TAKEN;
		}
		std::regex usernameRegex("^[a-zA-Z0-9]{3,20}$");
		if (!std::regex_match(username, usernameRegex)) {
			return SignInResult::INVALID_USERNAME;
		}

		std::regex passwordRegex("^[A-Za-z\\d]{8,}$");
		if (!std::regex_match(password, passwordRegex)) {
			return SignInResult::INVALID_PASSWORD;
		}

		std::string hashedPassword = hashPassword(password);
		if (!saveUser(username, hashedPassword)) {
			return SignInResult::INVALID_USERNAME; 
		}

		existingUsernames.insert(username);
		storedPasswords[username] = hashedPassword;
		return SignInResult::SUCCESS;
	}*/

	/**
	 * @brief Loguje istniej�cego u�ytkownika.
	 * @param username Nazwa u�ytkownika.
	 * @param password Has�o.
	 * @return Wynik operacji logowania.
	 */
	LogInResult logIn(const std::string& username, const std::string& password); /*{
		if (std::ranges::find(existingUsernames, username) == existingUsernames.end()) {
			return LogInResult::USER_NOT_FOUND;
		}
		std::string hashedPassword = hashPassword(password);
		if (storedPasswords.find(username)->second != hashedPassword) {
			return LogInResult::INCORRECT_PASSWORD;
		}
		return LogInResult::SUCCESS;
	}*/
};