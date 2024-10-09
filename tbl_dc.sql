-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Tempo de geração: 09/10/2024 às 06:35
-- Versão do servidor: 10.4.32-MariaDB
-- Versão do PHP: 8.2.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Banco de dados: `db_iot`
--

-- --------------------------------------------------------

--
-- Estrutura para tabela `tbl_dc`
--

CREATE TABLE `tbl_dc` (
  `id` int(11) NOT NULL,
  `data` varchar(12) NOT NULL,
  `hora` varchar(9) NOT NULL,
  `temp` varchar(5) NOT NULL,
  `umidade` varchar(5) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Despejando dados para a tabela `tbl_dc`
--

INSERT INTO `tbl_dc` (`id`, `data`, `hora`, `temp`, `umidade`) VALUES
(1, ' 08/10/2024\n', ' 23:35:49', ' 25.8', ' 58.0'),
(2, ' 08/10/2024\n', ' 23:43:25', ' 25.8', ' 59.0'),
(3, ' 08/10/2024\n', ' 23:43:33', ' 25.8', ' 59.0'),
(4, ' 09/10/2024\n', ' 00:57:14', ' 25.4', ' 59.0'),
(5, ' 09/10/2024\n', ' 01:01:04', ' 25.8', ' 58.0');

--
-- Índices para tabelas despejadas
--

--
-- Índices de tabela `tbl_dc`
--
ALTER TABLE `tbl_dc`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT para tabelas despejadas
--

--
-- AUTO_INCREMENT de tabela `tbl_dc`
--
ALTER TABLE `tbl_dc`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
